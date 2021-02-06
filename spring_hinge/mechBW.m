% status feb 6 1am: currently, the min and max power is off. Trying
% different spring sizes from mcm carr, aiming roughly for ~1W power draw.
% Trying to maximize lifting ability for other hardware, like dust cover. 

%mechsys dynamics
g = 1.62; %m/s2, gravity on moon/earth
m = 1.65; %kg, antenna mass

L = 0.290; %m, antenna length
realI = 1/3.*m.*L.^2; %kg m2, moment of inertia
theta0 = 225 * pi/180; %undeformed spring angle, in radians
K = 1.146; %Nm/rad, spring stiffness

K = 14*2*0.112984829/theta0

earthMass = m * g/9.81;
earthI = 1/3*earthMass*L.^2;
requiredI = realI - earthI;
requiredMass = m - earthMass;
inertialD = 0.025;
rho = 7800;
steelLen = 2*requiredI./(pi*(inertialD/2).^4.*rho);

BW = sqrt(K./realI)

% motor characteristics
% not looking at any dynamics, just the "stall torque" and current required
% to hold a given spring at fully retracted. 

V = 12; %volts, motor voltage
%K_motor = (0.700 - 0.300)/(0.5 - 0.25); % Nm/A, motor torque constant, from interpolation on datasheet
T_max = 0.8; % Nm, based on stall torque in datasheet.
I_max = 0.6; % A, based on stall current in datasheet
pulley_radius = 0.007; % mm, pulley radius
tension_min = (theta0 + pi/2).*K./(L/2*sin(pi/4)); %N, min cable tension, at top. No gravity torque 
tension_max = ((theta0 + pi).*K - m.*g.*L/2)./(L/2); %N, max cable tension, at bottom, supported by gravity. simulink
motor_torque_max = tension_max*pulley_radius; % Nm, torque required to hold antenna full closed
motor_torque_min = tension_min*pulley_radius;
motor_current_max = motor_torque_max/T_max*I_max; % Amps, current required to hold antenna fully closed. Assumes that it will be proportional to ration between required torque and stall torque at 12V. 
motor_current_min = motor_torque_min/T_max*I_max;
motor_power_max = V*(motor_current_max/I_max)*motor_current_max % W, power draw to hold position. Same as above, voltage scales down by proportion of required current and max current.
motor_power_min = V*(motor_current_max/I_max)*motor_current_min

