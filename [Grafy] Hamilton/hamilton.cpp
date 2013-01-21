#include <iostream>  
#include <fstream>  
#include <string>  
#include <vector> 
using namespace std;  
vector<int> sprawdz(vector< vector<int> > graf, vector<int> sciezka);  
vector<int> kn(vector< vector<int> > graf, vector<int> sciezka);  // if k0 <n
vector<int> roz(vector< vector<int> > graf, vector<int> sciezka);  //if k(s) < n
vector<int> odwr(vector< vector<int> > graf, vector<int> sciezka);  //if k(s) < n odwrocona
vector<int> cykl(vector< vector<int> > graf, vector<int> sciezka);  // k(s)
vector<int> sort(vector<vector<int> > graf); 
vector<vector<int> >  
            reindex(vector<vector<int> > graf, vector<int> index); 
ifstream infile ("graf.txt");     
ofstream outfile("sciezki.txt");     
int main()  
{  
 int i, j, k, n, vertex, krw;  
 infile>>n;                        //Wczytanie liczby wierzcho³ków
 vector< vector<int> > graf;       //Wczytanie macierzy s¹siedztwa  
 for(i=0; i<n; i++)  
 {  
  vector<int> row;  
  for(j=0; j<n; j++)  
  {  
   infile>>krw;  
   row.push_back(krw);  
  }  
  graf.push_back(row);  
 }

 vector<int> index=sort(graf); 
 graf=reindex(graf,index);

 for(vertex=0; vertex<n; vertex++)  //Loop through all vertices 
 {  
  vector<int> sciezka;  
  sciezka.push_back(vertex);           //Select initial vertex 
  sciezka=sprawdz(graf,sciezka);     //Part I  
  sciezka=kn(graf,sciezka);     //Part II 
  k=sciezka.size();  
  if(k<n)   {sciezka=roz(graf,sciezka); k=sciezka.size();} 
  if(k<n)   {sciezka=odwr(graf,sciezka); k=sciezka.size();} 
  if(k<n) outfile<<"sciezka("<<k<<"): "; 
  else outfile<<"Graf: ";//Part III 
  for(i=0; i<sciezka.size(); i++) outfile<<index[sciezka[i]]+1<<" "; 
  outfile<<endl;  
  if(k==n)  
  {  
   vector<int> tw_cykl=cykl(graf,sciezka);  
   if(!tw_cykl.empty())  
   {  
    for(j=0; j<tw_cykl.size(); j++)  
    {  
     outfile<<"Cykl Hamiltona w grafie:\t";  
     for(k=0; k<=tw_cykl[j]; k++)  
      outfile<<index[sciezka[k]]+1<<" "; 
     for(k=n-1; k>tw_cykl[j]; k--)  
      outfile<<index[sciezka[k]]+1<<" "; 
     outfile<<endl;  
    }  
   }  
   outfile<<endl;  
  }  
 }  
 cout<<"sciezki.txt zapisane"<<endl;  
  system("PAUSE");  
  return 0;  
} 

//funkcja odwiedzaj¹ca wierzcho³ek vi.. vk  i sprawdza czy vk jest odwiedzony
vector<int> sprawdz(vector< vector<int> > graf, vector<int> sciezka)  
{  
 int i, j, k, n=graf.size();  
 vector<int> rozsz_sciezka;  
 vector<int> odwiedzony;  
 for(i=0; i<n; i++)  
  odwiedzony.push_back(0);  
 int akt;  
 for(i=0; i<sciezka.size(); i++)  
 {  
  akt=sciezka[i];  
  odwiedzony[akt]=1;  
  rozsz_sciezka.push_back(akt);  
 }  
 for(k=0; k<n; k++)  
 {  
  vector<int> sasiad;  
  for(i=0; i<n; i++)  
   if(graf[akt][i]==1 && odwiedzony[i]==0)  
    sasiad.push_back(i);  
   if(!sasiad.empty())  
   {  
    int wybor=sasiad[0];  
    int minimum=n;  
    for(i=0; i<sasiad.size(); i++)  
    {  
     vector<int> next_sasiad;  
     for(j=0; j<n; j++)  
      if(graf[sasiad[i]][j]==1 && odwiedzony[j]==0)  
       next_sasiad.push_back(j);  
      int eta=next_sasiad.size();  
      if(eta<minimum)  
      {  
       wybor=sasiad[i];  
       minimum=eta;  
      }  
    }  
    akt=wybor;  
    odwiedzony[akt]=1;  
    rozsz_sciezka.push_back(akt);  
   }  
   else break;  
 }  
 return rozsz_sciezka;  
} 

vector<int> kn(vector< vector<int> > graf, vector<int> sciezka)  
{  
 int i, j, k, n=graf.size(); 
 bool quit=false; 
 while(quit!=true)  
 {  
 int m=sciezka.size(), inlet=-1, outlet=-1; 
  vector<int> sasiad;  
  for(i=0; i<sciezka.size(); i++)  
   if(graf[sciezka[m-1]][sciezka[i]]==1) sasiad.push_back(i);  
   vector<int> nieodwiedzony;  
   for(i=0; i<n; i++)  
   {  
    bool outside=true;  
    for(j=0; j<sciezka.size(); j++)  
     if(i==sciezka[j]) outside=false;  
     if(outside==true) nieodwiedzony.push_back(i);  
   }  
   if((!nieodwiedzony.empty()) && (!sasiad.empty()))  
   {  
    int maximum=0;  
    for(i=0; i<sasiad.size(); i++)  
     for(j=0; j<nieodwiedzony.size(); j++)  
      if(graf[sciezka[sasiad[i]+1]][nieodwiedzony[j]]==1)  
      {  
       vector<int> next_sasiad;  
       for(k=0; k<nieodwiedzony.size(); k++)  
       if(graf[nieodwiedzony[j]][nieodwiedzony[k]]==1)  
         next_sasiad.push_back(nieodwiedzony[k]);  
       int eta=next_sasiad.size();  
       if(eta>=maximum)  
        {  
         inlet=sasiad[i];  
         outlet=nieodwiedzony[j];  
         maximum=eta;  
        }  
      }  
   }  
   vector<int> rozsz_sciezka;  
   if(inlet!=-1 && outlet!=-1)  
   {  
    for(i=0; i<=inlet; i++)  
     rozsz_sciezka.push_back(sciezka[i]);  
    for(i=sciezka.size()-1; i>inlet; i--)  
     rozsz_sciezka.push_back(sciezka[i]);  
    rozsz_sciezka.push_back(outlet);  
   }  
   if(!rozsz_sciezka.empty()) sciezka=rozsz_sciezka;  
   if(m<sciezka.size()) sciezka=sprawdz(graf,sciezka);  
   else quit=true;  
 } 
 return sciezka;  
} 
//rozszerzenie sciezki przez znalezienie vi
vector<int> roz(vector< vector<int> > graf, vector<int> sciezka) 
{  
 int i, j, k, l, p, n=graf.size(); 
 bool quit=false; 
 while(quit!=true)  
 {  
  vector<int> rozsz_sciezka;  
  int m=sciezka.size(); 
  vector<int> nieodwiedzony; 
  for(i=0; i<n; i++) 
  { 
    bool outside=true;  
    for(j=0; j<sciezka.size(); j++)  
     if(i==sciezka[j]) outside=false;  
    if(outside==true) nieodwiedzony.push_back(i); 
  } 
  bool big_check=false; 
  for(i=0; i<sciezka.size(); i++) 
  { 
    for(j=0; j<nieodwiedzony.size(); j++) 
    { 
     if(graf[nieodwiedzony[j]][sciezka[i]]==1) 
     { 
       vector<int> pom_sciezka; 
       pom_sciezka.push_back(nieodwiedzony[j]); 
       vector<int> pom_rozsz_sciezka; 
       vector<int> pom_odwiedzony; 
       for(l=0; l<n; l++) 
       pom_odwiedzony.push_back(0); 
       int akt; 
       for(l=0; l<pom_sciezka.size(); l++) 
       { 
       akt=pom_sciezka[l]; 
        pom_odwiedzony[akt]=1; 
        pom_rozsz_sciezka.push_back(akt); 
       } 
       for(l=0; l<n; l++) 
       { 
       bool unfound=true; 
       for(k=0; k<nieodwiedzony.size(); k++) 
        if(l==nieodwiedzony[k]) unfound=false; 
       if(unfound==true) pom_odwiedzony[l]=1; 
       } 
       for(l=0; l<n; l++) 
       { 
        vector<int> sasiad; 
       for(l=0; l<n; l++) 
       if(graf[akt][l]==1 && pom_odwiedzony[l]==0) 
        sasiad.push_back(l); 
       if(!sasiad.empty()) 
        { 
         int wybor=sasiad[0]; 
         int minimum=n; 
         for(l=0; l<sasiad.size(); l++) 
          { 
           vector<int> next_sasiad; 
          for(k=0; k<n; k++) 
           if(graf[sasiad[l]][k]==1 && pom_odwiedzony[k]==0) 
            next_sasiad.push_back(k); 
           int eta=next_sasiad.size(); 
           if(eta<minimum) 
            { 
             wybor=sasiad[l]; 
             minimum=eta; 
            } 
          } 
          akt=wybor; 
          pom_odwiedzony[akt]=1; 
         pom_rozsz_sciezka.push_back(akt); 
        } 
       else break; 
     } 
     int last_vertex=pom_rozsz_sciezka[pom_rozsz_sciezka.size()-1]; 
     int vj; 
     bool check=false; 
     while(check==false && !pom_rozsz_sciezka.empty()) 
     { 
     for(p=sciezka.size()-2; p>i; p--) 
     { 
      if(graf[sciezka[p]][last_vertex]==1  
         && graf[sciezka[i+1]][sciezka[p+1]]==1) 
      { 
       check=true; 
       vj=p; 
       break; 
      } 
     } 
     if(check==false) 
     { 
      pom_rozsz_sciezka.pop_back(); 
      last_vertex=pom_rozsz_sciezka[pom_rozsz_sciezka.size()-1]; 
     } 
     } 
     if(check==true) 
     { 
      vector<int> pom; 
      for(p=0; p<=i; p++) 
      pom.push_back(sciezka[p]); 
      for(p=0; p<pom_rozsz_sciezka.size(); p++) 
      pom.push_back(pom_rozsz_sciezka[p]); 
      for(p=vj; p>i; p--) 
      pom.push_back(sciezka[p]); 
      for(p=vj+1; p<sciezka.size(); p++) 
      pom.push_back(sciezka[p]); 
      pom_rozsz_sciezka=pom; 
      big_check=true; 
      rozsz_sciezka=pom_rozsz_sciezka; 
     } 
     } 
    } 
     if(big_check==true) 
     { 
      break; 
     } 
  } 
   if(!rozsz_sciezka.empty()) sciezka=rozsz_sciezka;  
   if(m<sciezka.size()) 
   { 
    sciezka=sprawdz(graf,sciezka); 
    sciezka=kn(graf,sciezka); 
   } 
   else quit=true;  
 } 
 return sciezka;  
} 
//funkcja dla odwroconej sciezki
vector<int> odwr(vector< vector<int> > graf, vector<int> sciezka) 
{  
  vector<int> odwrocona_sciezka; 
  for(int i=sciezka.size()-1; i>=0; i--) odwrocona_sciezka.push_back(sciezka[i]); 
  odwrocona_sciezka=roz(graf,odwrocona_sciezka); 
  return odwrocona_sciezka; 
}
//funkcja znajdujaca cykle hamiltona do odpowiednich sciezek
vector<int> cykl(vector< vector<int> > graf, vector<int> sciezka)  
{ 

 int i, n=sciezka.size();

 vector<int> tw_cykl;  
 for(i=0; i<n-1; i++)  
  if((graf[sciezka[0]][sciezka[i+1]]==1) && (graf[sciezka[i]][sciezka[n-1]]==1)) 
   tw_cykl.push_back(i); 
 return tw_cykl;  
}

vector<int> sort(vector<vector<int> > graf) 
{ 
 int i, j; 
 vector<int> degree; 
 for(i=0; i<graf.size(); i++) 
 { 
  int sum=0; 
  for(j=0; j<graf[i].size(); j++) 
  if(graf[i][j]==1) sum++; 
  degree.push_back(sum); 
 } 
 vector<int> index; 
 for(i=0; i<degree.size(); i++) index.push_back(i); 
 for(i=0; i<degree.size(); i++) 
 for(j=i+1; j<degree.size(); j++) 
 if(degree[i]<degree[j]) swap(index[i],index[j]); 
 return index; 
}

vector<vector<int> >  
      reindex(vector<vector<int> > graf, vector<int> index) 
{ 
  int i, j; 
  vector<vector<int> > pom=graf; 
  for(i=0; i<pom.size(); i++) 
  for(j=0; j<pom[i].size(); j++) 
  pom[i][j]=graf[index[i]][index[j]]; 
  return pom; 
}