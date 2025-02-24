clear variables; % clear all variables
close all;       % close all figures
clc;             % clear console


data1 = readmatrix("testing-table2.txt");
x1 = data1(3:5:end, 2);
y1 = data1(3:5:end, 3);

data2 = readmatrix("testing-mtf.txt");
x2 = data2(3:5:end, 2);
y2 = data2(3:5:end, 3);

data3 = readmatrix("testing-array.txt");
x3 = data3(3:5:end, 2);
y3 = data3(3:5:end, 3);

figure
subplot(3,1,1)
plot(x1, y1, "-x")
hold on
plot(x2, y2, "-x")
plot(x3, y3, "-x")
title("Lookups with non-existent keys")
xlabel("n-värde")
ylabel("ms")

grid on
hold off

legend("Table2", "Mtf", "Array", Location="northwest")

% 

data1 = readmatrix("testing-table2.txt");
x1 = data1(4:5:end, 2);
y1 = data1(4:5:end, 3);

data2 = readmatrix("testing-mtf.txt");
x2 = data2(4:5:end, 2);
y2 = data2(4:5:end, 3);

data3 = readmatrix("testing-array.txt");
x3 = data3(4:5:end, 2);
y3 = data3(4:5:end, 3);

subplot(3,1,2)
plot(x1, y1, "-x")
hold on
plot(x2, y2, "-x")
plot(x3, y3, "-x")
title("Random lookups")
xlabel("n-värde")
ylabel("ms")

grid on
hold off

legend("Table2", "Mtf", "Array", Location="northwest")

% 

data1 = readmatrix("testing-table2.txt");
x1 = data1(5:5:end, 2);
y1 = data1(5:5:end, 3);

data2 = readmatrix("testing-mtf.txt");
x2 = data2(5:5:end, 2);
y2 = data2(5:5:end, 3);

data3 = readmatrix("testing-array.txt");
x3 = data3(5:5:end, 2);
y3 = data3(5:5:end, 3);

subplot(3,1,3)
plot(x1, y1, "-x")
hold on
plot(x2, y2, "-x")
plot(x3, y3, "-x")
title("Skewed lookups")
xlabel("n-värde")
ylabel("ms")

grid on
hold off

legend("Table2", "Mtf", "Array", Location="northwest")