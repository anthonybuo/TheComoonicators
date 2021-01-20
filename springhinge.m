L = 0.1;
Y = 570E6; %As-rolled 1095 https://www.theworldmaterial.com/sae-aisi-1095-high-carbon-steel/
E = 110E9; %https://www.theworldmaterial.com/sae-aisi-1095-high-carbon-steel/
t = 0.005*0.0254;
b = 0.28;
I = b*t^3/12;

cog = 0.28/2;
m = 1.65;
g = 1.62;

R = [0.1:0.1:3.5]; %Force vector
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
    prba(i) = solve(eqn, theta);  % pseduo-rigid body angle
    torque(i) = prba(i) * K(i); % torque
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
