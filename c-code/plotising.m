a = importdata('ising.out').data;

subplot(2, 1, 1);
plot(a(:, 1), a(:, 2), '*-');
title('Energie pro Spin');

subplot(2, 1, 2);
plot(a(:, 1), a(:, 3), '*-');
title('Magnetisierung pro Spin');