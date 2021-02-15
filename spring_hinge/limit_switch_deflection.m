% Simulate the deflection of the flexible limit switch mechanism. Change
% the material parameters until the desired deflection is reached. The
% textbook "Handbook of Compliant Mechanisms" sections 4.4.1 and A1.1 were
% used.

%% Set up
clear all
clc

%% Constants from design
azimuth_torque = 0.279; % From stepper datasheet
pinion_radius = 0.05 / 2;  % From CAD
ring_force = azimuth_torque / pinion_radius;

%% Simulation 1: Cantilever with Force at Tip Model
N = 100;
R = linspace(ring_force*0.9, ring_force*1.1, N); %N - end load magnitude
psi = 90*ones(1, N); % degrees - direction of end load
t = 0.0008128; %m - perpendicular to bending axis
b = 0.02; %m - parallel to bending axis
L = 0.06; %m - beam length
E = 210E9;

I = b*t^3/12; %m^4 - 2nd moment of area

[~, ~, x, y, M1] = fpbeambending(R,psi,L,E,I,100);

figure(1)
clf
subplot(1, 2, 1)
plot(x,y)
grid on;
title('Tip Deflection for Various Applied Forces Using Simple Fixed-Force Model')
xlabel('X Position (mm)')
ylabel('Y Position (mm)')

%% Simulation 2: Small-Length Flexural Pivot Model
N = 100; % num samples
l = linspace(5e-3, 35e-3, N); % m - length of flexible portion
L = linspace(45e-3, 15e-3, N); % m - length of stiff portion
b = 0.2; % m - dimension parallel to bending axis
torque = ring_force.*L;
spring_steel_thicknesses = [5e-3 10e-3 15e-3 20e-3 25e-3 32e-3] * 25.4 / 1000; % mm

subplot(1, 2, 2)
plot(L+l, 0, 'r.', 'MarkerSize', 20);
hold on;
for i = 1:size(spring_steel_thicknesses,2)
    t = spring_steel_thicknesses(i); % m - dimension perpendicular to bending axis
    I = b*t^3/12; % m^4 - 2nd moment of area
    K = E*I./l; % equivalent torsional stiffness
    theta = torque ./ K; % pseudo- rigid body angle
    a = l/2 + (L + l/2).*cos(theta); % tip deflection x
    b = (L + l/2).*sin(theta); % tip deflection y
    plot(a, b, '.', 'MarkerSize', 20, 'color', [1-0.1*i 0.1*i 0]);
    hold on;
end

grid on;
title('Tip Deflection for Varying Geometries Using Small-Length Flexural Pivot')
xlabel('X Position (mm)');
ylabel('Y Position (mm)');
max_stress = ring_force .* a .* b./2 / I;
