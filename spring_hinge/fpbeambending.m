function [a,b,x,y,M1] = fpbeambending(R,psi,L,E,I,numpts)
%This function finds the deflection of a fixed-pinned beam using elliptic
%integrals. 
% Inputs:
% R - a vector with values for force
% psi - a vector (same size as R) with values for force direction, in
% degrees
% L - Length of the beam (scalar)
% E - Young's modulus of the beam (scalar)
% I - Second moment of area of the beam's cross section (scalar)
% numpts - a scalar value giving the number of points to include in x and y
%
% Outputs:
% a - the horizontal component of the beam's end (a vector the length of R)
% b - the vertical component of the beam's end (a vector the length of R)
% x - a matrix (numpts rows and as many columns as the length of R) giving
% horizontal components for points along the beam's shape
% y - a matrix the same size as x givng vertical components for points
% along the beam's shape
% M1 - the moment reaction at the fixed end of the beam (a vector the
% length of R)

psi = psi*pi/180; %convert to radians
alpha = R*L^2/(E*I);
k = zeros(size(alpha));

for counter = 1:length(alpha)
    k(counter) = fzero(@alphaeq,[abs(cos(psi(counter)/2)) 1-eps],[],alpha(counter),psi(counter));
end
phi1 = asin(cos(psi/2)./k);
phi2 = pi/2*ones(size(phi1));
[f1,e1] = elliptic12(phi1,k.^2);
[f2,e2] = elliptic12(phi2,k.^2);
a = -(cos(psi).*(f1 - f2 + 2*(e2 - e1)) + ...
    2*sin(psi).*k.*(cos(phi2) - cos(phi1)))./sqrt(alpha);
b = -(sin(psi).*(f1 - f2 + 2*(e2 - e1)) + ...
    2*k.*cos(psi).*(cos(phi1)-cos(phi2)))./sqrt(alpha);
[x,y,s,beta,theta] = guidedbeamshape(psi,phi1,phi2,k,alpha,f1,e1,numpts);
beta1 = beta(1,:);
a = a*L;
b = b*L;
x = x*L;
y = y*L;
M1 = beta1*E*I/L;

function eqout = alphaeq(k,alpha,psi)
phi1 = asin(cos(psi/2)/k); %eq 4.5
if isreal(phi1)
else phi1 = pi/2;
end
phi2 = pi/2; %for fixed beam only
[f1,e1] = elliptic12(phi1,k.^2);
[f2,e2] = elliptic12(phi2,k.^2);
eqout = f2 - f1 - sqrt(alpha);


function [x,y,s,beta,theta] = guidedbeamshape(psi,phi1,phi2,k,alpha,f1,e1,npts)
if size(phi1,1)>1
    psi = psi';
    phi1 = phi1';
    phi2 = phi2';
    k = k';
    alpha = alpha';
    f1 = f1';
    e1 = e1';
end

numpts = npts-1;
phi = zeros(numpts+1,length(phi1));
for counter = 1:length(phi1)
phi(:,counter) = (phi1(counter):(phi2(counter) - phi1(counter))/numpts:phi2(counter))';
end
temp2 = ones(size(phi,1),1);
if length(psi)==1
    psi = psi*ones(size(phi1));
end
[f2,e2] = elliptic12(phi,temp2*(k.^2));
x = -(temp2*cos(psi).*(2*(e2 - temp2*e1)-f2 + temp2*f1) ...
    + 2*(temp2*sin(psi)).*(temp2*k).*(cos(phi) - cos(temp2*phi1)))./sqrt(temp2*alpha);
y = -((temp2*sin(psi)).*(2*(e2 - temp2*e1)-f2 + temp2*f1) - ...
    2*(temp2*k).*(temp2*cos(psi)).*(cos(phi) - cos(temp2*phi1)))./sqrt(temp2*alpha);
s = (f2 - temp2*f1)./sqrt(temp2*alpha);
beta = 2*(temp2*k).*(temp2*sqrt(alpha)).*cos(phi);
theta = 2*(asin((temp2*k).*sin(phi))-asin((temp2*k).*sin(temp2*phi1)));

function [F,E,Z] = elliptic12(u,m,tol)
% ELLIPTIC12 evaluates the value of the Incomplete Elliptic Integrals 
% of the First, Second Kind and Jacobi's Zeta Function.
%
%   [F,E,Z] = ELLIPTIC12(U,M,TOL) where U is a phase in radians, 0<M<1 is 
%   the module and TOL is the tolerance (optional). Default value for 
%   the tolerance is eps = 2.220e-16.
%
%   ELLIPTIC12 uses the method of the Arithmetic-Geometric Mean 
%   and Descending Landen Transformation described in [1] Ch. 17.6,
%   to determine the value of the Incomplete Elliptic Integrals 
%   of the First, Second Kind and Jacobi's Zeta Function [1], [2].
%
%       F(phi,m) = int(1/sqrt(1-m*sin(t)^2), t=0..phi);
%       E(phi,m) = int(sqrt(1-m*sin(t)^2), t=0..phi);
%       Z(phi,m) = E(u,m) - E(m)/K(m)*F(phi,m).
%
%   Tables generating code ([1], pp. 613-621):
%       [phi,alpha] = meshgrid(0:5:90, 0:2:90);                  % modulus and phase in degrees
%       [F,E,Z] = elliptic12(pi/180*phi, sin(pi/180*alpha).^2);  % values of integrals
%
%   See also ELLIPKE, ELLIPJ, ELLIPTIC3, THETA, AGM.
%
%   References:
%   [1] M. Abramowitz and I.A. Stegun, "Handbook of Mathematical Functions", 
%       Dover Publications", 1965, Ch. 17.1 - 17.6 (by L.M. Milne-Thomson).
%   [2] D. F. Lawden, "Elliptic Functions and Applications"
%       Springer-Verlag, vol. 80, 1989

%   For support, please reply to 
%       moiseev[at]sissa.it, moiseev.igor[at]gmail.com
%       Moiseev Igor, 
%       34106, SISSA, via Beirut n. 2-4,  Trieste, Italy
%
%   The code is optimized for ordered inputs produced by the functions 
%   meshgrid, ndgrid. To obtain maximum performace (up to 30%) for singleton, 
%   1-dimensional and random arrays remark call of the function unique(.) 
%   and edit further code. 

if nargin<3, tol = eps; end
if nargin<2, error('Not enough input arguments.'); end

if ~isreal(u) || ~isreal(m)
    error('Input arguments must be real.')
end

if length(m)==1, m = m(ones(size(u))); end
if length(u)==1, u = u(ones(size(m))); end
if ~isequal(size(m),size(u)), error('U and M must be the same size.'); end

F = zeros(size(u)); 
E = F;              
Z = E;
m = m(:).';    % make a row vector
u = u(:).';

if any(m < 0) || any(m > 1), error('M must be in the range 0 <= M <= 1.'); end

I = uint32( find(m ~= 1 & m ~= 0) );
if ~isempty(I)
    [mu,J,K] = unique(m(I));   % extracts unique values from m
    K = uint32(K);
    mumax = length(mu);
    signU = sign(u(I));

    % pre-allocate space and augment if needed
	chunk = 7;
	a = zeros(chunk,mumax);
	c = a; 
	b = a;
	a(1,:) = ones(1,mumax);
	c(1,:) = sqrt(mu);
	b(1,:) = sqrt(1-mu);
	n = uint32( zeros(1,mumax) );
	i = 1;
	while any(abs(c(i,:)) > tol)                                    % Arithmetic-Geometric Mean of A, B and C
        i = i + 1;
        if i > size(a,1)
          a = [a; zeros(2,mumax)];
          b = [b; zeros(2,mumax)];
          c = [c; zeros(2,mumax)];
        end
        a(i,:) = 0.5 * (a(i-1,:) + b(i-1,:));
        b(i,:) = sqrt(a(i-1,:) .* b(i-1,:));
        c(i,:) = 0.5 * (a(i-1,:) - b(i-1,:));
        in = uint32( find((abs(c(i,:)) <= tol) & (abs(c(i-1,:)) > tol)) );
        if ~isempty(in)
          [mi,ni] = size(in);
          n(in) = ones(mi,ni)*(i-1);
        end
	end
     
    mmax = length(I);
	mn = double(max(n));
	phin = zeros(1,mmax);     C  = zeros(1,mmax);    
	Cp = C;  e  = uint32(C);  phin(:) = signU.*u(I);
	i = 0;   c2 = c.^2;
	while i < mn                                                    % Descending Landen Transformation 
        i = i + 1;
        in = uint32(find(n(K) > i));
        if ~isempty(in)     
            phin(in) = atan(b(i,K(in))./a(i,K(in)).*tan(phin(in))) + ...
                pi.*ceil(phin(in)/pi - 0.5) + phin(in);
            e(in) = 2.^(i-1) ;
            C(in) = C(in)  + double(e(in(1)))*c2(i,K(in));
            Cp(in)= Cp(in) + c(i+1,K(in)).*sin(phin(in));  
        end
	end
    
    Ff = phin ./ (a(mn,K).*double(e)*2);                                                      
    F(I) = Ff.*signU;                                               % Incomplete Ell. Int. of the First Kind
    Z(I) = Cp.*signU;                                               % Jacobi Zeta Function
    E(I) = (Cp + (1 - 1/2*C) .* Ff).*signU;                         % Incomplete Ell. Int. of the Second Kind
end

% Special cases: m == {0, 1}
m0 = find(m == 0);
if ~isempty(m0), F(m0) = u(m0); E(m0) = u(m0); Z(m0) = 0; end

m1 = find(m == 1);
um1 = abs(u(m1)); 
if ~isempty(m1), 
    N = floor( (um1+pi/2)/pi );  
    M = find(um1 < pi/2);              
    
    F(m1(M)) = log(tan(pi/4 + u(m1(M))/2));   
    F(m1(um1 >= pi/2)) = Inf.*sign(u(m1(um1 >= pi/2)));
    
    E(m1) = ((-1).^N .* sin(um1) + 2*N).*sign(u(m1)); 
    
    Z(m1) = (-1).^N .* sin(u(m1));                      
end