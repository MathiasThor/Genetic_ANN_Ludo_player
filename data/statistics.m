 % Paired t-test
% sigma unknown or small sample size n < 30

% The function ttest2 tests if two independent samples come from normal 
% distributions with equal but unknown standard deviations and the same 
% mean, against the alternative that the means are unequal.

% This is however a righ tailed test, that tests if x is greater than y.

close; clc; clear all:

% ANN vs. SS
% data_own   = [2330 2359 2375 2373 2409 2400 2396 2388 2285 2384];
% data_other = [2556 2547 2541 2542 2530 2533 2534 2537 2571 2538];

% SS
% data_own   = [5673 5998 5973 5978 6040 5955 6088 6018 5886 6001];
% data_other = [1442 1334 1342 1340 1320 1348 1304 1327 1371 1333];

% ANN
data_own   = [5011 4950 5086 4979 5020 5020 4988 5052 4971 5094];
data_other = [1663 1683 1638 1673 1660 1660 1670 1649 1676 1635];

% Q
%data_own   = [];
%data_other = [];

mean_own = mean(data_own);
var_own  = var(data_own);

mean_other = mean(data_other);
var_other  = var(data_other);

fprintf('Mean: %.2f', mean_own/100)

figure(1)
subplot(1,2,1)
normplot(data_own)
subplot(1,2,2)
normplot(data_other)

[h,p,ci,stat] = ttest2(data_own, data_other);
fprintf('\nThe P value is: %d\n', p)
fprintf('Rejected (1) or accepted (0) at 5%% signifance level: %i\n\n', h)
stat


