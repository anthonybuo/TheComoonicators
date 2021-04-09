function angle = angleBtwVects(v1, v2)
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here
angle = atan2(norm(cross(v1,v2)), dot(v1,v2));
end

