set terminal png
set output "write.png"

set xrange[-30:500]
set xtics 64
set boxwidth 10
plot "foo.dat" u 1:3:2:6:5 with candlesticks
