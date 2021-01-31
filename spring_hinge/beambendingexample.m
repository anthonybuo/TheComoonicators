R = [500:500:8000];%N - end load magnitude
psi = -179*ones(size(R));%degrees - direction of end load
t = 0.01;%m - in-plane beam thickness
b = 0.05;%m - out-of-plane beam width
L = 1;%m - beam length
E = 200e9;%Pa - Young's modulus

I = b*t^3/12;%m^4 - 2nd moment of area

[a,b,x,y,M1] = fpbeambending(R,psi,L,E,I,100);

figure(1)
clf
plot(x,y)
axis equal