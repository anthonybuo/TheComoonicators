global L
global R
global E
global I

L = 0.1;
Y = 570E6; %As-rolled 1095 https://www.theworldmaterial.com/sae-aisi-1095-high-carbon-steel/
E = 210E9; %https://www.theworldmaterial.com/sae-aisi-1095-high-carbon-steel/
t = 0.005*0.0254;
b = 0.28;
I = b*t^3/12;

cog = 0.28/2;
m = 1.65;
g = 1.62;

R = [0.1:0.1:3.5]; %#ok<NBRAK> %Force vector
psi = 135*ones(size(R)); %force angle vector (degrees)
numputs = 1000;

[a,b,x,y,M1] = fpbeambending(R,psi,L,E,I,numputs);

subplot(2,2,1);
plot(x,y);
title("L = " + num2str(L) + ", t = " + num2str(t));
xlabel("xpos (m)");
ylabel("ypos (m)");
axis equal;
legend(num2str(R'));

endAngle = atan2(y(end,:)-y(end-1,:),x(end,:)-x(end-1,:))*180/pi;
endVect = [y(end, :);x(end, :)];
appTorque = R.*sqrt(endVect(1,:).^2+endVect(2,:).^2).*sind(psi(1,:)-endAngle);

% subplot(2,2,4);
% plot(endAngle, appTorque);
% title("Torque vs angle");
% xlabel(" Bend Angle from static (deg)");
% ylabel("Torque (Nm)");

%%torsional approximation: page 77/68 real in handbook of compliant
%%mechanisms

n = 1./(tand(180-psi(1,:)));

gamma = zeros(size(psi));

for index = 1:length(gamma)
    if n(index) >= 0.5 && n(index) <= 10
        gamma(index) = 0.841655 - 0.0067807*n(index) + 0.000438*n(index)^2;
    elseif n(index) >= -1.8316 && n(index) <= 0.5
        gamma(index) = 0.852144 - 0.0182867*n(index);
    elseif -5 <= n(index) && n(index) <= -1.8316
        gamma(index) = 0.912364 + 0.0145928*n(index);
    else
        gamma(index) = NaN;
    end
end

K_t = zeros(size(psi));

for index = 1:length(gamma)
    if n(index) > -5 && n(index) <= -2.5
        K_t(index) = 3.024112 + 0.121290*n(index)+0.003169*n(index)^2;
    elseif n(index) > -2.5 && n(index) <= -1
        K_t(index) = 1.967647 - 2.616021*n(index)-3.738166*n(index)^2-2.649437*n(index)^3-0.891906*n(index)^4-0.113063*n(index)^5;
    elseif -1 < n(index) && n(index) <= 10
        K_t(index) = 2.654855 - 0.509896E-1*n(index)+0.126749E-1*n(index)^2-0.142039E-2*n(index)^3+0.584525E-4*n(index)^4;
    else
        K_t(index) = NaN;
    end
end


K = gamma.*K_t*E*I/L;

P = R./sqrt(1+n.^2);

%% Torque Attempt 1 using torsional stiffness * displacement angle
torque = zeros(size(psi));
prba = zeros(size(psi));
for i = 1:length(gamma)
    syms theta
    % Equation: A.17
    eqn = P(i) == K(i)*theta/(gamma(i)*L*(cos(theta)+n(i)*sin(theta)));
    prba(i) = vpasolve(eqn, theta);
    torque(i) = prba(i) * K(i);
end
subplot(2,2,4);
plot(torque, 'b.');
title("Torque vs Config");
xlabel("Configurations");
ylabel("Torque (Nm)");

%% Torque Attempt 2 using moment arm * force
torque = zeros(size(psi));
moment_arm_length = zeros(size(psi));
for i = 1:length(gamma)
    moment_arm_length(i) = sqrt(a(i)^2 + b(i)^2);
    torque(i) = moment_arm_length(i) * R(i) * cos(90 - prba(i) - 45);
end
hold on;
grid on;
plot(torque, 'r.');
legend('K*theta', 'Force*arm');

%% Torque Attempt 3 using combo moment and force
% Only calculating for configuration #35

% "Simulated Annealing Algorithm for Modeling Large Deflection of Flexible
% Links in Compliant Mechanisms" Eq. 4
PRB_3R_fn = @PRB_3R;
x0 = [0, 0, 0];
options = optimset('Display','off');
deflection = fsolve(PRB_3R_fn, x0, options);
torque = dot(deflection, [K_t1 K_t2 K_t3]);

hold on;
plot(35, torque, 'gx');

%% Torque Attempt 4 using applied moment only (case A.1.5 in compliant mechanisms handbook)

M = 1.25;
b = 0.28;
t = 0.01*0.0254;
L = 0.1;

[a_moment, b_moment, theta_moment, K_moment, sigmax_moment] = momentOnly(L, E, b*t^3/12, M, t);

safety_factor = Y/sigmax_moment
K_moment
theta_moment

%% Stress
stress = P.*(x(end,:)+n.*y(end,:))*(t/2)/I - n.*P/(t*b);

subplot(2,2,2);
hold on
plot(stress);
ylabel("Stress (Pa)");
hold off

subplot(2,2,3);
hold on
FoS = Y./stress;
plot(FoS, 'x');
ylabel("Factor of Safety");
xlabel("Configurations");
ylim([0,10]);
hold off;

%%
function [a, b, theta, K, sigmax] = momentOnly(L, E, I, M, t)
K = 1.5164*E*I/L;
theta = M/K;

a = L*(1-0.7346*(1-cos(theta)));
b = 0.7346*L*sin(theta);
theta = theta*1.5164*180/pi; %yes this is dumb but it should be theta and Theta
sigmax = M*t/2/I;
K = K/180*pi;
end
%%
% for col = 1:size(x,2)
%     
%     [len,rad,curvect] = curvature([x(:,col),y(:,col)]);
% 
%     stress = E./rad*t/2;
%     
% 
%     subplot(2,2,2);
%     hold on
%     plot(stress);
%     ylabel("Stress (Pa)");
%     hold off
%     
%     
%     subplot(2,2,3);
%     hold on
%     FoS = Y./stress;
%     plot(FoS);
%     xlabel("Factor of Safety");
%     ylim([0,10]);
%     hold off;
% end

% Computes the angular deflection of each link in the PRB 3R model using
% "Simulated Annealing Algorithm for Modeling Large Deflection of Flexible
% Links in Compliant Mechanisms" Eq. 4
function F = PRB_3R(x)
    global L
    global R
    global E
    global I
    
    % Table A.5.3, Chen coefficients
    gamma1 = 0.351;
    gamma2 = 0.388;
    gamma3 = 0.136;
    K_t1 = 3.25;
    K_t2 = 2.84;
    K_t3 = 2.95;
    
    % "Simulated Annealing Algorithm for Modeling Large Deflection of Flexible
    % Links in Compliant Mechanisms" Eq. 2
    M0 = 3.5*0.15;  % 3.5N @ 150mm
    alpha = sqrt(L^2*R(end)/(E*I));
    k = M0^2/(2*R(end)*E*I);

    F(1) = K_t1*x(1) - gamma3*cos(x(1)+x(2)+x(3))*alpha^2 - M0*L/E/I;
    F(2) = K_t2*x(2) - gamma2*cos(x(1)+x(2)) - gamma3*cos(x(1)+x(2)+x(3))*alpha^2 - M0*L/E/I;
    F(3) = K_t3*x(3) - gamma1*cos(x(1)+x(2)) - gamma2*cos(x(1)+x(2)) - gamma3*cos(x(1)+x(2)+x(3))*alpha^2 - M0*L/E/I;
end
