data = load('pdata.mat');
close all

data = table2array(data.a);

targets = data(:, 1:2);

targetAngles = zeros(size(targets));

d_o1 = 13.25; % cm
d_12 = 29.8; % cm
d_23 = 2.1; % cm
d_ow = 147+d_o1+d_23; % cm

for i = 1:length(data)
    angle = getAngles(targets(i,1),targets(i,2),d_o1, d_12, d_23, d_ow);
    targetAngles(i, :) = angle;
end

laserAngles = zeros(15, 6);
linError = zeros(size(laserAngles));
angError = zeros(size(laserAngles));
absAngError = zeros(size(aziError));

for i = [1, 3, 5]
    for j = 1:length(data)
        ii = (i-1)/2 + 1;
        laserAngles(j, i:i+1) = getAngles(data(j,i+2),data(j,i+3),d_o1, d_12, d_23, d_ow);
        angError(j,i:i+1) = targetAngles(j,:) - laserAngles(j, i:i+1);
        linError(j,i:i+1) = targets(j,:) - data(j,i+2:i+3);
        absAngError(j,ii) = getAngError(targets(j,1), targets(j,2), linError(j, 1), linError(j,2),d_o1, d_12, d_23, d_ow);
    end
end

aziError = zeros(15, 3);
aziError(:,1) = angError(:,1);
aziError(:,2) = angError(:,3);
aziError(:,3) = angError(:,5);

elevError = zeros(15, 3);
elevError(:,1) = angError(:,2);
elevError(:,2) = angError(:,4);
elevError(:,3) = angError(:,6);

aziErrorUnsorted = nonzeros(reshape(aziError, 1, []));
elevErrorUnsorted = nonzeros(reshape(elevError, 1, []));
absAngErrorUnsorted = nonzeros(reshape(absAngError, 1, []));


figure;
histogram(aziErrorUnsorted * 180/pi);
title("Azimuth Error, 3\sigma = " + 3*std(aziErrorUnsorted * 180 / pi));
xlabel("Error (degrees)");
ylabel("Trials");

figure;
histogram(elevErrorUnsorted * 180/pi);
title("Elevation Error, 3\sigma = " + 3*std(elevErrorUnsorted * 180 / pi));
xlabel("Error (degrees)");
ylabel("Trials");


figure;
h1 = histogram(absAngErrorUnsorted * 180/pi);
title("Absolute Error, 3\sigma = " + 3*std(absAngErrorUnsorted * 180 / pi));
xlabel("Error (degrees)");
ylabel("Trials");
h1.BinWidth = 0.1;
