% Paired t-test
% sigma unknown or small sample size n < 30

% The function ttest2 tests if two independent samples come from normal 
% distributions with equal but unknown standard deviations and the same 
% mean, against the alternative that the means are unequal.

% This is however a righ tailed test, that tests if x is greater than y.

close; clc; clear all:

% ANN vs. SS
%data_own   = [];
%data_other = [];

% SS
%data_own   = [5581 5599 5499 5542 5479 5549 5451 5486 5484 5517];
%data_other = [1473 1473 1473 1473 1507 1483 1516 1504 1505 1494];

% ANN
data_own   = [4594 4642 4680 4612 4687 4665 4550 4641 4661 4617];
data_other = [1802 1786 1773 1796 1771 1778 1816 1786 1779 1794];

% Q
%data_own   = [];
%data_other = [];

mean_own = mean(data_own);
var_own  = var(data_own);

mean_other = mean(data_other);
var_other  = var(data_other);

figure(1)
subplot(1,2,1)
normplot(data_own)
subplot(1,2,2)
normplot(data_other)

[h,p] = ttest2(data_own,data_other, 'Tail', 'right');
fprintf('\nThe P value is: %d\n', p)
fprintf('Rejected (1) or accepted (0) at 5%% signifance level: %i\n\n', h)


