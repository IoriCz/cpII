function teilchen

% Ortsvektor x und Geschwindigkeitsvektor v haben jeweils
% 2 Komponenten fuer raeumliche x- und y-Koordinate
r = [ 0 0 ];
v = [ 1 0 ];

dt = .5;
nStep = 200;

figure(1)
clf

% In bahn werden die Ortsvektoren aufgesammelt
bahn = r;

for step = 1:nStep
    % Drift-Kick-Schritt
    r = r + dt *v;
    v = boris(r, v, dt);

    % Darstellung der Bahn
    bahn = [ bahn ; r];
    plot(bahn(:, 1), bahn(:, 2), '-*');
    xlabel('x'); ylabel('y');
    pause(0.1)
end

end

% Berechnet aus alter Geschwindigkeit und Position
% die neue Geschwindigkeit.
% x und v_alt sind hier jeweils 2-er Vektoren in der
% x-y-Ebene!
% Es ist auf q/m = 1 normiert.
function v_neu = boris(r, v_alt, dt)
% Felder am Teilchenort (nur relevante Komponenten!):
ex = 0.;
bz = 1.;

v_minus = v_alt + dt/2 * [ ex 0 ];
tz = dt/2 * bz;
v_strich =  v_minus + kreuz(v_minus, tz);
v_plus = v_minus + 2/(1+tz^2) * kreuz(v_strich, tz);
v_neu = v_plus + dt/2 * [ ex 0 ];

% Test:
v_alt + dt*( [ ex 0] + 0.5*kreuz(v_alt+v_neu, bz) ) - v_neu
end

% Kreuzprodukt aus einem Vektor a in der x-y-Ebene und
% einem Vektor in z-Richtung. Ergebnis liegt wieder in
% x-y-Ebene.
% Nur diese Operation wird bei der vorliegenden Geometrie
% benoetigt.
function k = kreuz(a, bz)
k = bz*[ a(2) -a(1) ];
end