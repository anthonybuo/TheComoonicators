function [angError, distances] = getAngError(targetx, targety, dx, dy, d_o1, d_12, d_23, d_ow)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here

dirFirstVector = getAngles(targetx, targety, d_o1, d_12, d_23, d_ow);
dirSecondVector = getAngles(targetx+dx, targety+dy, d_o1, d_12, d_23, d_ow);

posFirstVector = getPos(dirFirstVector(1), dirFirstVector(2), d_o1, d_12, d_23);
posSecondVector = getPos(dirSecondVector(1), dirSecondVector(2), d_o1, d_12, d_23);

endPosFirstVector = getPos(dirFirstVector(1), dirFirstVector(2), d_o1, d_12, d_23 + 100);
endPosSecondVector = getPos(dirSecondVector(1), dirSecondVector(2), d_o1, d_12, d_23 + 100);

vectorStartPoints = transpose([posFirstVector, posSecondVector]);
vectorEndPoints = transpose([endPosFirstVector, endPosSecondVector]);

[P_intersect, distances] = lineIntersect3D(vectorStartPoints, vectorEndPoints);

FirstVector = posFirstVector - transpose(P_intersect);
SecondVector = posSecondVector - transpose(P_intersect);

angError = angleBtwVects(FirstVector, SecondVector);
end

