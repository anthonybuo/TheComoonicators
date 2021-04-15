function [angError] = getAngError(targetx, targety, dx, dy, d_o1, d_12, d_23, d_ow)
% This function calculates the angular error between a target position and
% a laser position, found by measuring its xy coordinates off the target
% position
% inputs:
% targetx, targety: the x and y positions of the target on the wall
% dx, dy: the measured distance in x, y from the target to the laser
% position on the wall
% d_o1: azimuth (origin) to hinge link length (scalar)
% d_12: hinge to laser link length in antenna plane (scalar)
% d_23: laser base to laser tip link length (scalar)
% d_ow: azimuth (origin) to wall length in z vector (scalar)
%
% outputs:
% angError: the estimated absolute angle, radians, between the two vectors the 
% antenna needs to point along to hit the target vs the position it hit

dirFirstVector = getAngles(targetx, targety, d_o1, d_12, d_23, d_ow);
dirSecondVector = getAngles(targetx+dx, targety+dy, d_o1, d_12, d_23, d_ow);

% no consideration of my original coordinate system. I am moving the
% vectors to the origin, calculating some kind of xyz vector based off
% the azi and elev angles, then finding the angle between them
a = [sin(dirFirstVector(1))*cos(dirFirstVector(2)), cos(dirFirstVector(1))*cos(dirFirstVector(2)), sin(dirFirstVector(2))];
b = [sin(dirSecondVector(1))*cos(dirSecondVector(2)), cos(dirSecondVector(1))*cos(dirSecondVector(2)), sin(dirSecondVector(2))];
angError = atan2(norm(cross(a,b)), dot(a,b));

% angError = angleBtwVects(FirstVector, SecondVector);
end

