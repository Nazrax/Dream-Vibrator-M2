#!/usr/bin/perl

use strict;
use warnings;

my (@avgX, @avgY, @avgZ);
initArray(\@avgX);
initArray(\@avgY);
initArray(\@avgZ);

open IH, $ARGV[0] or die;
my $ticks = 0;

while (1) {
  # Something's wrong if packets don't come in pairs
  #my ($packet1, $packet2);
  #my $count = sysread IH, $packet1, 128;
  #last unless $count == 128;

  #$count = sysread IH, $packet2, 128;
  #last unless $count == 128;

  #my $packet = $packet1 . $packet2;

  my $packet;
  my $count = sysread IH, $packet, 256;
  last unless $count == 256;

  my @packet = unpack("c*", $packet);

  my $i = 0;
  $i = 7 if ($packet[0] == -1 && $packet[1] == -18); # 0xFF and 0xEE, signed
  while ($i < @packet-2) {
    $ticks++;
    my ($x,$y,$z) = ($packet[$i], $packet[$i+1], $packet[$i+2]);
    my $ax = updateAvgs($x, \@avgX);
    my $ay = updateAvgs($y, \@avgY);
    my $az = updateAvgs($z, \@avgZ);
    print "$ticks $x $ax $y $ay $z $az\n";
    $i += 3;
  }

  #while ($i < @packet) {
  #  $ticks++;
  #  print "$ticks $i $packet[$i]\n";
  #  $i++;
  #}
}

sub initArray {
  my $ref = shift;
  for(my $i=0; $i<10; $i++) {
    $ref->[$i] = 0;
  }
}

sub updateAvgs {
  my $data = shift;
  my $ref = shift;
  pop @$ref;
  unshift @$ref, $data;
  my $sum = 0;
  for(my $i=0; $i<@$ref; $i++) {
    $sum += $ref->[$i];
  }
  return 1.0 * $sum / @$ref;
}
