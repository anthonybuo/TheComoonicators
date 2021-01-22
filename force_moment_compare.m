% This script analyzes a beam with a pure moment applied versus a pure force 
% applied to the end. It finds spring constants, safety factors, and beam
% dimensions for the two loading considitions based on solving pseudo rigid 
% body models bending to a specified angle - typically 90 degrees.
%
% I have confirmed it produces the correct and position of the beam
% compared to the elliptic integral method for a bend angle of 45 degrees,
% L = 0.1, t = 0.005 in, b = 0.28. 

L = 0.075;
Y = 570E6; %As-rolled 1095 https://www.theworldmaterial.com/sae-aisi-1095-high-carbon-steel/
E = 210E9; %https://www.theworldmaterial.com/sae-aisi-1095-high-carbon-steel/
t = 0.005*0.0254;
b = 0.28;
I = b*t^3/12;
target_K = 20E-3; %Nm/deg
target_SF = 1;
height_limit = 0.05;

bendangle = 90; %degrees

n_vs_c_theta = [0.0 0.5 1.0 1.5 2.0 3.0 4.0 5.0 7.5 10.0 -0.5 -1.0 -1.5 -2.0 -3.0 -4.0 -5.0;...
    1.2385 1.2430 1.2467 1.2492 1.2511 1.2534 1.2548 1.2557 1.2570 1.2578 1.2348 1.2323 1.2322 1.2293 1.2119 1.1971 1.1788];


%% Force
psi = [100:10:170]; %force angle vector (degrees)
numputs = 1000;

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

for index = 1:length(K_t)
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

K_force = gamma.*K_t*E*I/L;

c_theta = interp1(n_vs_c_theta(1,:), n_vs_c_theta(2,:), n);

tempangle = bendangle./c_theta*pi/180;

P = K_force.*tempangle./(gamma*L.*(cos(tempangle)+n.*sin(tempangle)));

a_force = L*(1-gamma.*(1-cos(tempangle)));
b_force = gamma.*L.*sin(tempangle);

stressmax_force = ones(2, length(gamma));
stressmax_force(1,:) = abs(P.*(a_force + n.*b)*t/2/I - n.*P/(b*t));
stressmax_force(2,:) = abs(-P.*(a_force + n.*b)*t/2/I - n.*P/(b*t));


safety_factor_force = ones(size(gamma));
for index = 1:length(safety_factor_force)
    safety_factor_force(index) = Y/max(stressmax_force(:,index));
end

K_force = K_force*pi/180;

%% Moment

K_moment = 1.5164*E*I/L; %Nm/rad
M = K_moment*bendangle*pi/180;

a_moment = L*(1-0.7346*(1-cosd(bendangle/1.5164)));
b_moment = 0.7346*L*sind(bendangle/1.5164);
stressmax_moment = M*t/2/I;
K_moment = K_moment/180*pi;

safety_factor_moment = Y/stressmax_moment;
K_moment; %Nm per degree


%% present results
subplot(1,3,1);
sgtitle("L = " + L + "m, b = " + b + "m, t = " + t*1000 + "mm, Bend Angle = " + bendangle + " degrees");
plot(psi, safety_factor_force, psi, safety_factor_moment*ones(size(psi)), psi, ones(size(psi)), '*');
ylim([0,5]);
xlabel("Applied force angle (degrees)");
ylabel("Factor of Safety");
legend("Force", "Moment", "Target SF");
hold off

subplot(1,3,2);
plot(psi, K_force, psi, K_moment*ones(size(psi)), psi, target_K*ones(size(psi)), '*');
xlabel("Applied force angle (degrees)");
ylabel("Effective Spring Constant (Nm/deg)");
legend("Force", "Moment", "Target Spring Constant");

subplot(1,3,3);
plot(psi, a_force, psi, a_moment*ones(size(psi)), psi, height_limit*ones(size(psi)), '*');
xlabel("Applied force angle (degrees)");
ylabel("bent spring height");
legend("Force", "Moment", "Height limit");