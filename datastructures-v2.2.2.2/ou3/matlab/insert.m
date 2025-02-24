clear variables; % clear all variables
close all;       % close all figures
clc;             % clear console

data1 = readmatrix("testing-table2.txt");
x1 = data1(1:5:end, 2);
y1 = data1(1:5:end, 3);

data2 = readmatrix("testing-mtf.txt");
x2 = data2(1:5:end, 2);
y2 = data2(1:5:end, 3);

data3 = readmatrix("testing-array.txt");
x3 = data3(1:5:end, 2);
y3 = data3(1:5:end, 3);


figure


subplot(2,1,1)
plot(x1, y1, "-x")
hold on
plot(x2, y2, "-x")

xlabel("n-värde")
ylabel("ms")
grid on
hold off
legend("Table2", "Mtf", Locaion="northwest")

subplot(2,1,2)
plot(x3, y3, "-x", Color="#EDB120")

xlabel("n-värde")
ylabel("ms")
grid on
legend("Array", Location="northwest")