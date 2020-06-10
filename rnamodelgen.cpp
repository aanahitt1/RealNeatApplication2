#include "rnamodelgen.h"

RNAModelGen::RNAModelGen()
{

}

QFile* RNAModelGen::generate2DModel(const QString fasta_file){

    //check to make sure file is a valid file
    if(!QFileInfo::exists(fasta_file) && QFileInfo(fasta_file).isFile())
        qCritical() << "RNAModelError: " << "File " << fasta_file << "is not a valid file.";

    rnaPlotCmd = "C:\\Program Files (x86)\\ViennaRNA Package\\RNAplot.exe";
    rnaPlotArgs << "-o" << "svg" << fasta_file;

    // start RNAplot
    rnaPlot = new QProcess(this);
    rnaPlot->setProgram(rnaPlotCmd);
    rnaPlot->setArguments(rnaPlotArgs);
    rnaPlot->setProcessChannelMode(QProcess::ForwardedChannels);
    rnaPlot->start(QIODevice::ReadWrite);

    if(!rnaPlot->waitForFinished()){
        qCritical() << "RNAPlot failed to finish";
        qCritical() << rnaPlot->errorString();
    }


    //open svg file saved (output of RNAPlot)
    modelName = validateFasta(fasta_file);
   QString rna_o = QDir().absolutePath() + "/" + modelName.append("_ss.svg");
    QFile rnaplot_output(rna_o);



    //create unique file to write structure (picture) to
    QTemporaryFile* unique_svg = new QTemporaryFile("rnaPictXXXXXX.svg");
      if(!unique_svg->open()){
           qCritical() << "Did not open temp file!";
       }

     modelFile = unique_svg->fileName();


    if(!rnaplot_output.open(QIODevice::ReadWrite)){
        qCritical() << "Can't open RNAPlot output file";
    }

    //copy contents of RNAPlot output to unique file
    unique_svg->write(rnaplot_output.readAll());
    unique_svg->waitForReadyRead(-1);


    rnaplot_output.close();
    unique_svg->close();

    if(!QFile::remove(rna_o))
        qWarning() << "Could not remove RNAPlot SVG file";


    return unique_svg;

}

// checks the given structure file to ensure it has a fasta header
// and if it does the 'name' in the header is returned
QString RNAModelGen::validateFasta(const QString filepath){
    QFile sec_strc(filepath);

    if(!sec_strc.open(QIODevice::ReadWrite)){
        qCritical() << sec_strc.errorString();
        return "";
    }

    QTextStream input(&sec_strc);
    while(!input.atEnd()){

        QString line = input.readLine();

        if(!line.trimmed().isEmpty()){ //look at first line that has text

            // for a fasta file with a header line that looks like this:
            // >myseq ...potentially other text...
            //return "myseq"
            if(line.trimmed().startsWith(">")){
                return line.simplified().section(" ", 0, 0).remove(">");
            }else
              break;
        }

    }

    qCritical() << "Structure file invalid";
    return "";
}

QFile* RNAModelGen::highlight2DModel(const QString& svgFile, const QString& secStruc1,const QString& secStruc2){

    QHash<int,bool>* highlights = getHighlights(secStruc1, secStruc2);
    if(highlights == NULL)
        return NULL;

    QFile* svg_xml = new QFile(svgFile);
    if(!svg_xml->open(QIODevice::ReadWrite) ){
        qCritical() << "Could not open SVG XML file";
        svg_xml->close();
        return NULL;
    }

    QDomDocument svgXMLDoc("structure_svg");
    if(!svgXMLDoc.setContent(svg_xml)){
        qCritical() << "Could not load XML DOM";
        return NULL;
    }


    QDomNodeList graphics = svgXMLDoc.elementsByTagName("g");
    QDomNode graphicsRoot = graphics.at(0);
    QDomNode graph_bases = graphics.at(2);
    QDomNodeList pline = svgXMLDoc.elementsByTagName("polyline");
    QDomElement polyline = pline.at(0).toElement();
    QDomNodeList base_specs = graph_bases.childNodes();

    //create highlights
    QDomElement highlightElements = svgXMLDoc.createElement("g");
    highlightElements.setAttribute("id", "highlights");


// draw circles for the bases that have a different structure compared
// to last plotted sequence
    for(int i=0; i<base_specs.size(); i++){

        if(highlights->contains(i)){
        QDomElement base = base_specs.at(i).toElement();

        QDomElement hl = svgXMLDoc.createElement("circle");
        hl.setAttribute("cx", base.attributeNode("x").value());
        hl.setAttribute("cy",base.attributeNode("y").value());
        hl.setAttribute("r","7");
        hl.setAttribute("fill","yellow");
        highlightElements.appendChild(hl);
        }

    }

     graphicsRoot.insertBefore(highlightElements,polyline);

//overwrite SVG file with the version containing highlightsS
     svg_xml->close();
     svg_xml->open(QIODevice::ReadWrite | QIODevice::Truncate);
     QTextStream stream( svg_xml );
     stream << svgXMLDoc.toString();

    return svg_xml;
}

// check which structure portions (if any) are different from the last plotted
// sequence and record the location of the portions which are different
QHash<int,bool>* RNAModelGen::getHighlights(const QString &struc1, const QString &struc2){
    QFile ss1(struc1);
    QFile ss2(struc2);
    if(! (ss1.open(QIODevice::ReadOnly) && ss2.open(QIODevice::ReadOnly))){
        qCritical() << "Could not open file for structure comparison";
        return NULL;
    }

    if(!advanceToStrucLine(ss1)){
        qCritical() << "File " << ss1.fileName() << "has no structure";
        return NULL;
    }

    if(!advanceToStrucLine(ss2)){
        qCritical() << "File " << ss2.fileName() << "has no structure";
        return NULL;
    }

    char c1, c2;
    QTextStream in1(&ss1);
    QTextStream in2(&ss2);

    QHash<int,bool>* highlights = new QHash<int,bool>();
    int i = 0;
    while(! (in1.atEnd() || in2.atEnd()) ){
        in1 >> c1;
        in2 >> c2;

        // check to see if the structure line has ended
        // if the stuctr line has only ended for one
        // they are not the same length
        if(c1 == '\n' && c2 == '\n')
            break;
        if(c1 == '\n' || c2 == '\n'){
            qCritical() << "Structures not same length";
            return NULL;
        }

        if( c1 != c2)
            highlights->insert(i,true);
        i++;
    }


    return highlights;

}

// Takes a QFile holding a sec struct fasta file and
// advances it to the line with the structure
bool RNAModelGen::advanceToStrucLine(QFile& ss){
    char c1;
    bool ss_OnSeqLine = false;
    while(!ss_OnSeqLine){

        ss.readLine();
        ss.peek(&c1, 1);

        if(c1 == '.' || c1 == '(' || c1 == '[')
            ss_OnSeqLine = true;

        if(ss.atEnd()){
            return false;
        }
    }
    return true;
}

QString RNAModelGen::get2DModelFile(){
    return modelFile;
}

QString RNAModelGen::get2DModelName(){
    return modelName;
}

void RNAModelGen::processError(QProcess::ProcessError error){
    qInfo() << error;
}


