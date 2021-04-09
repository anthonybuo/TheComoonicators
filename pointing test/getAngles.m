function angles = getAngles(xdes, ydes, d_o1, d_12, d_23, d_ow)
%UNTITLED2 Summary of this function goes here
%   
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

