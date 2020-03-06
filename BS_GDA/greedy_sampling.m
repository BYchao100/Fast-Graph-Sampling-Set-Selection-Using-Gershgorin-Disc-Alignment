function [ selected_pebbles,vf ] = greedy_sampling( d,neis,neis_n,neis_w,n,mu,thres,K,pebbles_order,p_hops )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

[ ripples,q_nums]=computing_sets(neis,neis_n,neis_w,d,int32(n),mu,thres,int32(p_hops));
[selected_pebbles,vf]=solving_set_covering(ripples,q_nums,int32(n),int32(pebbles_order),int32(K));

end





