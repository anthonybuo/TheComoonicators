% Simulate the deflection of the flexible limit switch mechanism. Change
% the material parameters until the desired deflection is reached.

azimuth_torque = 0.279; % From stepper datasheet
pinion_radius = 0.05 / 2;  % From CAD
ring_force = azimuth_torque / pinion_radius;

R = ring_force; %N - end load magnitude
psi = 90; % degrees - direction of end load
t = 0.0008128; %m - perpendicular to bending axis
b = 0.01; %m - parallel to bending axis
L = 0.06; %m - beam length
E = 210E9;

I = b*t^3/12; %m^4 - 2nd moment of area

[a,b,x,y,M1] = fpbeambending(R,psi,L,E,I,100);

figure(1)
clf
plot(x,y)
axis equal