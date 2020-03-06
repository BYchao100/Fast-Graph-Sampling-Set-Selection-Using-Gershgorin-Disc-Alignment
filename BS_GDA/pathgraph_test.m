clear;
close all;

%% path graph generation
n=1000; % number of nodes on a path graph

A=zeros(n,n);
v1=ones(n-1,1);
A1=diag(v1,1);
A=A+A1+A1';

d=sum(A,2);

clear v1 A1;

%% test BS-GDA graph sampling

K=n*0.1; % sampling budget
mu=0.01; % mu 
epsilon=1e-5; % epsilon 
is_shuffle_nodes=true; % shuffling nodes before greedy sampling

% construct adjcency list for BS_GDA function from adjcency matrix
[ adj_list ] = sparse_neighbours_preparation( A,n );

% BS_GDA algorithm
tic;
[ selected_pebbles,thres ] = BS_GDA( adj_list,K,mu,epsilon,is_shuffle_nodes);
t=toc;
fprintf('BS-GDA running time:%f s\n',t);
fprintf('sampling:%d nodes, threshold:%f\n',numel(selected_pebbles),thres);

%% plot sampling nodes
graph_nodes=zeros(n,1);
graph_nodes(selected_pebbles)=1;
stem(graph_nodes,'-o');
ylim([0 2]);
xlabel('node id');
title('sampling on path graph');

%% compute the smallest eigenvalue via eigen-decomposition
L=diag(d)-A;
a=zeros(n,1);
a(selected_pebbles)=1;

B=diag(a)+mu*L;

B=sparse(B);
[~,se]=eigs(B,1,'sm');
fprintf('smallest eigenvalue via eigen-decomposition: %f \n',se);