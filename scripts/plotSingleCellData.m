function plotSingleCellData(file)
%PLOTSINGLECELLDATA Plots the data from a single cell simulation
%   Inputs:
%    - file, name of the file to read
src_dir = "/home/mroe734/Documents/shared/testoutput/SingleCellSimulationTest/";

table = readtable(src_dir + file);
mat = table2array(table);

plot(mat(:, 1), mat(:, 2));
end