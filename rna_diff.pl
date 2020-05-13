#!/usr/bin/env perl -w
# highlight_rna_diff.pl ---

use strict;

use IO::File;
use feature ':5.12';

sub HELP_MESSAGE {
  print STDERR "\nusage: $0 FOO_ss.fa FOO2_ss.fa FOO_ss.ps > FOO_new.ps\n";
}

HELP_MESSAGE() unless $#ARGV > 1;

my $fasta1 = IO::File->new(shift, O_RDONLY);
my $fasta2 = IO::File->new(shift, O_RDONLY);

# advance fasta files two lines (the dot-bracket notation
# starts after the second newline
<$fasta1>;
<$fasta1>;
<$fasta2>;
<$fasta2>;

my @hl_arr; #array for storing which bases should be highlighted

# go through each file up to 1KB at a time and compare the sections
# to each other char by char

while( $fasta1->read(my $s1, 1024) and $fasta2->read(my $s2, 1024) ){

  #turn strings into arrays
  my @a1 = split(//, $s1);
  my @a2 = split(//, $s2);

  die "Sequences don't match" unless $#a1 == $#a2;
  
    foreach my $char1 (@a1){
      my $char2 = shift @a2;

      # compare each string char by char
      if($char1 eq $char2){
        push @hl_arr, 'false';
      }else{
        push @hl_arr, 'true';
      }
    }
}

# Following code heavily edited code from ViennaRNA's relplot.pl
# Load and edit the PostScript file 
# the head and tail of document stay the same

my $macro_seen= 0;
my @ss_ps = ('','');    # array to store head and tail
my $n = swallow_ss_ps(); # read ss plot

print $ss_ps[0];     # print head

if (!$macro_seen) {
  print <<_E_O_F_ # use here-doc (super useful!)
/range 0.9 def
/drawreliability {
  /Smax 0.9 def
  0
  coor {
    aload pop
    S 3 index get
    dup
    {pop 1 1 0 setrgbcolor} {pop 1 1 1 setrgbcolor} ifelse 
    newpath
    fsize 2 div 0 360 arc
    fill
    1 add
  } forall
} bind def
_E_O_F_
}

# put the true/false values into PS file to
# specify which bases will be colored
print "/S [\n";
foreach (@hl_arr) {
  printf "  $_\n";
}
print "] def\n\n";
print "drawreliability\n";
print $ss_ps[1];  # print tail


sub swallow_ss_ps {
  # read the secondary structure plot
  my $length=0;
  my $tail=0;
  while (<>) {
    $macro_seen=1 if /drawreliability /;

    $length ++ if /^\/coor/ .. /^\] def/;

    $tail=1 if /^drawoutline/;
    $ss_ps[$tail] .= $_;
    last if eof;
  }
  return $length-2;
}
