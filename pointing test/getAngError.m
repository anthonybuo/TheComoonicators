function [angError] = getAngError(targetx, targety, dx, dy, d_o1, d_12, d_23, d_ow)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here

dirFirstVector = getAngles(targetx, targety, d_o1, d_12, d_23, d_ow);
dirSecondVector = getAngles(targetx+dx, targety+dy, d_o1, d_12, d_23, d_ow);

% no consideration of my original coordinate system. I am moving the
% vectors to the origin, calculating "some kind of" xyz position based off
% the azi and elev angles, then finding the angle between them
a = [sin(dirFirstVector(1))*cos(dirFirstVector(2)), cos(dirFirstVector(1))*cos(dirFirstVector(2)), sin(dirFirstVector(2))];
b = [sin(dirSecondVector(1))*cos(dirSecondVector(2)), cos(dirSecondVector(1))*cos(dirSecondVector(2)), sin(dirSecondVector(2))];
angError = atan2(norm(cross(a,b)), dot(a,b));

% angError = angleBtwVects(FirstVector, SecondVector);
end

