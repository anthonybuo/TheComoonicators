function laserpos = getPos(azi, elev, d_o1, d_12, d_23)
%UNTITLED8 Summary of this function goes here
%   Detailed explanation goes here

laserpos = [(d_o1-d_12*cos(elev)+d_23*sin(elev))*sin(azi);
            d_12*sin(elev)+d_23*cos(elev);
            (d_o1-d_12*cos(elev)+d_23*sin(elev))*cos(azi)];
end

