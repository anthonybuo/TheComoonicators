function angles = getAngles(xdes, ydes, d_o1, d_12, d_23, d_ow)
% This function calculates the azimuth and elevation angles required to
% point the antenna at a specific position on a wall.
%
% inputs:
% xdes, ydes: scalar x and y position on wall
% d_o1: azimuth (origin) to hinge link length (scalar)
% d_12: hinge to laser link length in antenna plane (scalar)
% d_23: laser base to laser tip link length (scalar)
% d_ow: azimuth (origin) to wall length in z vector (scalar)
% 
% outputs: [azimuth angle in radians, elevation angle in radians]
angles0 = [0, pi/2];

ydes = ydes + d_12;
[angles] = fsolve(@getPos2, angles0);    

    function xyz = getPos2(angles)
        laserpos = getPos(angles(1), angles(2), d_o1, d_12, d_23);

        laserend = getPos(angles(1), angles(2), d_o1, d_12, d_23+1000);

        wallnormal = [0;0;1];
        wallpoint = [0;0;d_ow];

        [I, check] = plane_line_intersect(wallnormal, wallpoint, laserpos, laserend);

        xyz(1) = I(1) - xdes;
        xyz(2) = I(2) - ydes;
        xyz(3) = I(3) - d_ow;
    end

end

