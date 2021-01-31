% Implementing the large deflection cantilever model for combined
% moment and force loading taken from "A LOAD INDEPENDENT
% PSEUDO-RIGID-BODY 3R MODEL FOR DETERMINING LARGE DEFLECTION OF
% BEAMS IN COMPLIANT MECHANISMS" chapter 3

% Setup
clear all
clc
clf

% Constants
c = 0.29;  % Antenna width [m]
N = 30;  % Num iterations

% Inputs
F = linspace(0.01, 10, N);    % Force applied by winch only [N], load index valid for F > 0
M = F .* c/2;                % Moment applied by winch only [Nm]
phi = linspace(deg2rad(100), deg2rad(170), N);  % Force angle (rad)

% Constants
earth_g = 9.81;           % m/s^2
antenna_mass = 5;         % kg
L = 0.078;                % Length
Y = 570e6;                % Yield stress
E = 210e9;                % Young's modulus
t = 0.01 * 0.0254;       % thickness
I = c*t^3/12;             % Inertia
alpha = F.*L^2./(2*E*I);  % Tip Angle due to force only
beta = M.*L./(E*I);       % Tip angle due to moment only
theta_0 = alpha + beta;   % Overall tip angle
rho = beta./2./alpha;     % load ratio
K = beta.^2./4./alpha;    % load index
s = 0.0001;                 % Quiver scaling factor (just for looks)

%% Update applied force and moment with weight of antenna
% New angle phi will be linear combo of original phi and pi based on their
% associated force magnitudes
phi = (pi * (antenna_mass * earth_g) + phi .* F) ./ sqrt(F.^2 + (antenna_mass * earth_g)^2);
% New applied force will be the vector addition of the original force and
% the weight of the antenna
F = sqrt((F.*cos(pi-phi) + (antenna_mass * earth_g)).^2 + (F.*sin(pi-phi)).^2);
% New applied moment is calculated from the new applied force
M = F .* c/2;

% Calculated below
stress = zeros(size(F));  % max stress (located at fixed end) for each iteration
safety_factor = zeros(size(F));
a = zeros(size(F));  % horizontal tip position
b = zeros(size(F));  % vertical tip position
alpha_sus = zeros(size(F));  % another way of calculating alpha

%% Main calc loop
figure(100);
for i = 1:N
    % Integrals in Eq9 and Eq10
    integral1 = @(x) 1     ./sqrt(cos(theta_0(i) - phi(i)) - cos(x - phi(i)) + K(i));
    integral2 = @(x) cos(x)./sqrt(cos(theta_0(i) - phi(i)) - cos(x - phi(i)) + K(i));
    integral3 = @(x) sin(x)./sqrt(cos(theta_0(i) - phi(i)) - cos(x - phi(i)) + K(i));

    % tip location
    alpha_sus(i) = (1/2 * integral(integral1, 0, theta_0(i)))^2;  % tip angle due to force only
    a(i) = L/2/sqrt(alpha_sus(i)) * integral(integral2, 0, theta_0(i));  % horizontal deflection of tip
    b(i) = L/2/sqrt(alpha_sus(i)) * integral(integral3, 0, theta_0(i));  % vertical deflection of tip
    
    % stress
    stress(i) = E*t/2/L*(beta(i) + 2*alpha_sus(i)*(a(i)/L*sin(phi(i)) - b(i)/L*cos(phi(i))));
    safety_factor(i) = Y / stress(i);
    
    % Plot results
    subplot(1, 2, 1);
    hold on;
    quiver(a(i), b(i), cos(theta_0(i)), sin(theta_0(i)), s*100, 'color', [1 0 0]);
    hold on;
    quiver(a(i), b(i), cos(phi(i)), sin(phi(i)), s*F(i), 'color', [0 0 1]);
    grid on;
end

%% Plot remaining results
sgtitle("Force+Moment Model: L = " + L + "m, b = " + c + "m, t = " + t*1000 + "mm");

subplot(1, 2, 1);
title('Tip Trajectory (black: tip position, red: tip angle, blue: force direction)');
xlabel('X Position (m)');
ylabel('Y Position (m)');
plot(a, b, 'k.', 'MarkerSize', 20)

subplot(1, 2, 2);
plot([1:1:N], safety_factor, 'g.-', 'MarkerSize', 15);
grid on;
title(sprintf('Safety Factor and Applied Force/Moment vs Iteration, Min(Safety Factor) = %.2f', min(safety_factor)));
ylabel('Safety Factor')
xlabel('Tip Angle (rad)')

yyaxis right
plot([1:1:N], M, 'b.-', 'MarkerSize', 15)
hold on
plot([1:1:N], F, 'r.-', 'MarkerSize', 15)
ylabel('Force (N) or Moment (Nm)')
xlabel('Iter')
grid on

legend('Safety Factor', 'Applied Moment', 'Applied Force')
%% Debug
% Why do the two ways of calculating alpha differ?
% for i = 1:N
%     % Percent error in alpha maybe
%     abs(alpha_sus(i) - alpha(i)) / alpha_sus(i)
% end
