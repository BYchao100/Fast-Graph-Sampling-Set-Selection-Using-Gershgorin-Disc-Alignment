function [ adj_list ] = sparse_neighbours_preparation( A,n )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

range=1:n;
neis=cell(n,1);
neis_w=cell(n,1);
neis_n=zeros(n,1,'int32');

d=sum(A,2);

for i=1:n
    neis{i,1}=int32(range(A(i,:)>0))-1;
    neis_w{i,1}=A(i,A(i,:)>0);
    neis_n(i)=sum(A(i,:)>0);
end

adj_list.neis=neis;
adj_list.neis_w=neis_w;
adj_list.neis_n=neis_n;
adj_list.d=d;

end



