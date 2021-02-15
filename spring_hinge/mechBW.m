% status feb 6 1am: currently, the min and max power is off. Trying
% different spring sizes from mcm carr, aiming roughly for ~1W power draw.
% Trying to maximize lifting ability for other hardware, like dust cover. 

%mechsys dynamics
close all
g = 1.62; %m/s2, gravity on moon/earth
m = 1.65; %kg, antenna mass

L = 0.290; %m, antenna length
realI = 1/3.*m.*L.^2; %kg m2, moment of inertia
T_in = linspace(0.1, 80, 20);
theta0_in = [360, 270, 225, 180, 120] * pi/180;
[T, theta0] = meshgrid(T_in, theta0_in); %undeformed spring angle, in radians; max spring load in in-lbs. These are values provided by mcmc

%find spring constant in Nm/rad for calculations
K = T*0.11298482933333./theta0;

% earthMass = m * g/9.81;
% earthI = 1/3*earthMass*L.^2;
% requiredI = realI - earthI;
% requiredMass = m - earthMass;
% inertialD = 0.025;
% rho = 7800;
% steelLen = 2*requiredI./(pi*(inertialD/2).^4.*rho);

BW = sqrt(K./realI);

% motor characteristics
% not looking at any dynamics, just the "stall torque" and current required
% to hold a given spring at fully retracted. 

V = 12; %volts, motor voltage
%K_motor = (0.700 - 0.300)/(0.5 - 0.25); % Nm/A, motor torque constant, from interpolation on datasheet
T_max = 0.8; % Nm, based on stall torque in datasheet.
I_max = 0.6; % A, based on stall current in datasheet
pulley_radius = 0.007; % mm, pulley radius
tension_min = (theta0 - pi/2).*K./(L/2*sin(pi/4)); %N, min cable tension, at top, but not perpendicular to plane. No gravity torque 
tension_max = ((theta0).*K - m.*g.*L/2)./(L/2); %N, max cable tension, at bottom, supported by gravity. simulink
motor_torque_max = tension_max*pulley_radius; % Nm, torque required to hold antenna full closed
motor_torque_min = tension_min*pulley_radius;
motor_current_max = motor_torque_max/T_max*I_max; % Amps, current required to hold antenna fully closed. Assumes that it will be proportional to ratio between required torque and stall torque at 12V. 
motor_current_min = motor_torque_min/T_max*I_max;
motor_power_max = V.*(motor_current_max./I_max).*motor_current_max; % W, power draw to hold position. Same as above, voltage scales down by proportion of required current and max current.
motor_power_min = V.*(motor_current_max./I_max).*motor_current_min;
if max(motor_current_max./I_max) > 1
    print("One configuration exceeds motor performance");
end

%% Find excess lifiting ability
% assume that lifted mass of cover increases linearly with elevation angle
% the cover will lift up off the ground at a constant rate with repsect to
% the arc length of the swinging edge of the antenna. I will account for
% this with the antenna mass + theta (0 to 90) * max dust cover mass 
%
% see page 130 of sam's logbook
% 
% basically, I've solved the torque balance between the spring torque
% (unstretched angle - elevation angle)*K; the gravity torque on the
% antenna (m*g*L/2*cos(theta); and the weight of the cover. 
% 
% The weight of the cover is modeled as "second winch line" whose tension increases as
% more of it is lifted off the ground. It connects to the edge of the
% antenna, creating a lever arm with length L. As the antenna elevates, 
% more dust cover lifts off the ground, but the angle decreases. The length
% is assumed to be the arc length swept out by the edge of the antenna.
% 
% This final term is W_cover/(pi*L/2)*sin(pi - theta/2)*L*(L/2*theta)
%                   {______} {______} {_______________}{_}{_________}
%                       ^       ^              ^        ^       ^
%               Total Weight    |              |        |       |
%                     total arc length         |        |       |
%                           Contact angle on antenna    |       |
%                                                   Lever Arm   |
%                                                         Arc length @
%                                                         elevation
% Equate everything and solve for W_cover, which is below.

theta = linspace(0, pi/2, 16);
max_dust_weight = zeros(size(K, 1), size(K, 2), length(theta));
danger_disturbance = (theta0 - pi/2).*K;
slack_shock = danger_disturbance*(2*realI-L^2)/(realI*m*L); % m/s^2, shock perpendicular to antenna req'd to cause slack in line. page 136 of sam's logbook. might be wrong.


for i = 1:length(theta)    
    max_dust_weight(:,:,i) = ((theta0 - theta(i)).*K- m*g*L/2.*cos(theta(i)))*pi/2*L./(sin(pi/2-theta(i)/2)*L^2/2*theta(i));
end
%% Plot stuff
% first plot shows the maximum power consumption for different spring
% consumptions. This is the power consumption to fully compress the spring,
% which is the point of maximum winch tension. This analysis ignore the
% weight of the dust cover. 
s = surf(theta0*180/pi, T, motor_power_max);
xlabel("Undeformed Position, degrees");
ylabel("Torque at full deformed, in-lbs");
zlabel("Max power consumption, watts");
s.EdgeColor = 'none';
figure(1);
cla;
% second plot shows the bandwidth for each spring combination, a simple
% root(K/M) for a torsional setup. A higher bandwidth is better
% Feb 11, 2021: Actually not accurate due to preload in string, omitting
% from output.
% s = surf(theta0*180/pi, T, BW);
% xlabel("Undeformed Position, degrees");
% ylabel("Torque at full deformed, in-lbs");
% zlabel("Mechanical Bandwidth, Hertz");
% s.EdgeColor = 'none';

% the third set of plots shows maximum dust cover weight for each spring
% combination at different elevation angles. Only the 90 degree/vertical
% plot is technically needed, since it offers the most conservative
% estimate for the current parameters. However, I imagine if the geometry
% of the problem changed, or we modeled torque required to lift the dust
% cover differently, you may end up with a different elevation angle being
% the "worst" case. Best to keep the subplots.
% The more dust cover you can lift, the better.

figure(2);
cla
for i = 1:length(theta)
    subplot(length(theta)/4, length(theta)/4, i)
    s = surf(theta0*180/pi, T, max_dust_weight(:,:,i));
    xlabel("Undeformed Position, degrees");
    ylabel("Torque at full deformed, in-lbs");
    zlabel("Max weight of dust cover, kg");
    title("Elevation angle = " + theta(i)*180/pi + " degrees")
    s.EdgeColor = 'none';
end

% fouth plot shows "danger disturbance torque" which causes cable to lose
% tension. Causes vibrations, potential tangling. Also shows acceleration
% of rover body req'd to cause loss of tension. 

figure(4);
cla;
s = surf(theta0*180/pi, T, danger_disturbance);
hold on
xlabel("Undeformed Position, degrees");
ylabel("Torque at full deformed, in-lbs");
zlabel("Disturbance torque (Nm), Disturbance Shock (m/s^2)");
title("Disturbance torque causing slack in line")
s.EdgeColor = 'none';
s = surf(theta0*180/pi, T, slack_shock);
legend(["Disturbance torque", "Rover Body Shock (edges)"]);

%% 
% fifth plot shows power consumption and disturbance withstance over all
% elevation angles for chosen spring

Spring_angle = 270;
theta0 = Spring_angle*pi/180;
K = 22.5*2*0.11298482933333./theta0;
gravity_torque = -m*g*L/2*cos(theta);
spring_torque = (theta0 - theta).*K;
net_torque = spring_torque + gravity_torque;

tension = net_torque./(L/2*sin(pi/2-theta/2)); %N, 
motor_torque = tension*pulley_radius; % Nm, torque required to hold antenna full closed
motor_current = motor_torque/T_max*I_max; % Amps, current required to hold antenna fully closed. Assumes that it will be proportional to ratio between required torque and stall torque at 12V. 
motor_power = V.*(motor_current./I_max).*motor_current; % W, power draw to hold position. Same as above, voltage scales down by proportion of required current and max current.



figure(5);
cla
hold on
yyaxis left;
h(1) = plot(theta*180/pi, motor_power);
ylabel("Power Consumption (W)");
ylim([-.1 inf])
yyaxis right;
h(2) = plot(theta*180/pi, spring_torque);
h(3) = plot(theta*180/pi, gravity_torque);
xlabel("Elevation Angle (degrees)");
ylabel("Torque (Nm)");
legend(h, ["Power Consumption" ,"Spring Torque", "Gravity Countertorque"], 'Location', 'east');
title(sprintf(["Spring hinge performance for \\theta_0 = %i" + char(176)+ ", \\tau_m_a_x = %i in-lb"], Spring_angle, 45))


