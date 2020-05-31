#include "optionchooser.h"

OptionChooser::OptionChooser(QList<QMap<QString, double>>* opt, QStringList* names)
{
    //This copies the opt list to options so we have the right size list.
    options = opt->at(0);

    //Create Dialog
    main = new QTabWidget();
    main->resize(300,300);
    for(int j = 0; j<names->length(); j++) {
            //This copies the opt list to options so we have the right size list.
            options = opt->at(j);

            //Create tab
            QWidget* window = new QWidget();
            QGridLayout* back = new QGridLayout(window);
            optList = new QListWidget();
            QPushButton* ok = new QPushButton("OK");
            QLabel* label = new QLabel("Options: ");
            QLabel* info = new QLabel("Please hit 'OK' on each tab to generate an image after choosing options, and exit when finished.");
            info->setWordWrap(true);
            back->cellRect(4, 1);
            back->addWidget(info, 1, 1);
            back->addWidget(label, 2, 1);
            back->addWidget(optList, 3, 1);
            back->addWidget(ok, 4, 1);

            connect(ok, &QPushButton::clicked, [=]() {
                emit boxClosed(options, j);
            });

            //Adds the items to the list
            int i =0;
            QMapIterator<QString, double> itr(options);
            while(itr.hasNext()) {
                itr.next();
                optList->addItem(itr.key());
                QListWidgetItem* item = optList->item(i);
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                item->setCheckState(Qt::Unchecked);
                i++;
            }
            main->addTab(window, names->at(j));

            connect(optList, &QListWidget::itemChanged, [=](QListWidgetItem* item) {getNumber(item);});
        }
}

void OptionChooser::getNumber(QListWidgetItem *item) {
    //This updates the map with the preference
    QString name = item->text();
    if(item->checkState() == Qt::Checked) {
        if(name.contains("Enter")) {
            double value(QInputDialog::getDouble(NULL, name, "Insert new value: "));
            options.insert(name, value);
        } else {
            options.insert(name, 1);
        }
    } else {
        options.insert(name, 0);
    }

}

//Literally just displays the window
void OptionChooser::show () {
    main->show();
}
