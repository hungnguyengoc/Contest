c[9999][9],v[2][9],p,T,K,Q,q,s,f;main(i,j){for(scanf("%d",&T);T--;p=0)for(scanf("%d%*d%d ",&K,&Q);Q--;){if(s=i=getchar()-49){for(;i++<K;)scanf("%d ",c[p]+i);++p;}else{for(;i<K*2;++i)scanf("%d ",v[i<K]+i%K);for(i=-1;++i<p;s+=!f*c[i][K])for(j=-1;++j<K&&!(f=c[i][j]<v[1][j]||c[i][j]>v[0][j]););printf("%d\n",s);}}}