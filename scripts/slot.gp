reset
set ylabel 'slots'
set style fill solid
set title 'hash distribution'
set term png enhanced font 'Verdana,10'
set output 'slot.png'
unset xtics
plot "slot.txt"
