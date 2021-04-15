function laserpos = getPos(azi, elev, d_o1, d_12, d_23)
% this function calculates the xyz coordinates of the antenna (of the laser
% ) given azimuth and elevation angles
% inputs:
% azi, elev: scalar azimuth and elelvation angles in degrees
% d_o1: azimuth (origin) to hinge link length (scalar)
% d_12: hinge to laser link length in antenna plane (scalar)
% d_23: laser base to laser tip link length (scalar)
% d_ow: azimuth (origin) to wall length in z vector (scalar)
%
% outputs:
% laserpos: a vector containing x, y, and z coordinates of the tip of the
% laser
laserpos = [(d_o1-d_12*cos(elev)+d_23*sin(elev))*sin(azi);
            d_12*sin(elev)+d_23*cos(elev);
            (d_o1-d_12*cos(elev)+d_23*sin(elev))*cos(azi)];
end

