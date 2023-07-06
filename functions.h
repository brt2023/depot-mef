#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__


#include <iostream>
#include "array.h"
#include "mesh.h"

#include <fstream>
#include <vector>
#include <cmath>
#include <cassert>
#include <string>


#define EPS 1.e-9
#define ITERMAX 1000


/* somme des elements d'un vecteur x */
inline double sum(Array& v, int istart,int iend){
    assert(v.n == 1);
    assert((istart >= 0) & (istart <= iend));
    assert((iend >= 0) & (iend >= istart));
    
    double s(0.0);
    for(int i=istart;i<iend;i++){
        s += v.x[i][0];
    }
    return s;
}

/* plus GRAND element entre deux valeurs a et b */
template <class T>
inline T MAX(T&a, T&b){
    return a>b?a:b;
}

/* plus PETIT element entre deux valeurs a et b */
template <class T>
inline T MIN(T&a, T&b){
    return a>b?b:a;
}









namespace solve{
    
    namespace directe{
        void LU(){ 
            std::cout << "\t\t\t\t en COURS de fabrication ..." << std::endl;
        }
    }//end namespace directe
    
    
    namespace iterative{
        void JACOBI(Array& A, Array& b){
            double s, delta = 1000;
            int m = A.m;
            Array x0(m,1),x1(m,1),x10(m,1),y0(m,1);
            
            //init.
            x0.initialize(0.0);//initialisation
            x0.display("x0");
            int iter = 0;
            while(delta > EPS){
                
                //x1[i][0] = (b[i][0] - sum[i][0])/A[i][i];
                for(int i=0;i<A.m;i++){
                    s = 0.0;
                    for(int j=0;j<A.n;j++){
                        if(j != i)
                            s += A.x[i][j]*x0.x[j][0];
                    }
                    x1.x[i][0] = (b.x[i][0] - s)/A.x[i][i];
                }

               
                if(iter > ITERMAX){
                    std::cout << "\t\tPas de CV !" << std::endl;
                    break;
                }
                
                x10 = x1 - x0;
//                 x10.display("x10");
                delta = sqrt((x10,x10));
                
                iter++;                
                x0 = x1;
                
                
            }//end while
            std::cout << "\n";
            std::cout << "\tApres " << iter << " iterations, avec une precision EPS = " << EPS << "\n";
            std::cout << "\tCV vers la solution = " << "\n";
            x0.display("x");
            
            
        }//end JACOBI
    }
    
    
    
    
    
    
    
    namespace projective{
        
        
        
        
        // methode projective
        // methode du GRADIENT CONJUGUE

        // On suppose que:
        // A est une matrice symetrique, definie positive
        void GC(Array& A,Array& b){
            //x0 points arbitraire
            int m = A.m;
            int n = A.n;
            Array x0(m,1),x1(m,1),r0(m,1),r1(m,1),p0(m,1),p1(m,1),x10(1,m);
            double alpha1,beta1;
            int iter;
            double delta = 1000;
            

            /* debut de l'algorithme */
            // 1-INIT.
            x0.initialize(0.0);
            x0.display("x0");            
            r0 = b - A*x0;//le residu
            p0 = r0;
            iter = 0;            
            do{
//                 std::cout << " --------- " << std::endl;
//                 std::cout << " iteration: " << iter + 1 << std::endl;

                alpha1 = (r0,r0)/(p0,A*p0);
//                 std::cout << "alpha1 = " << alpha1 << std::endl;
                
//                 x0.display("x0");
                
                x1 = x0 + p0*alpha1;
//                 x1.display("x1");
                
                x10 = x1 - x0;
//                 x10.display("x10");
                delta = sqrt((x10,x10));
//                 std::cout << "normeL2_delta = " << normeL2_delta << std::endl;
                
                
                
                r1 = r0 - (A*p0)*alpha1;
//                 r1.display("r1");
                
                beta1 = (r1,r1)/(r0,r0);
//                 std::cout << "beta1 = " << beta1 << std::endl;
                
                p1 = r1 + p0*beta1;
//                 p1.display("p1"); 
                
            
                if(iter > ITERMAX){
                    std::cout << "\t\tPas de CV !" << std::endl;
                    break;
                }
                
                iter ++;
                p0 = p1;
                x0 = x1;
                r0 = r1;                  


                
  
                
            }while(delta > EPS);//end while

            
            std::cout << "\n";
            std::cout << "\tApres " << iter << " iterations, avec une precision EPS = " << EPS << "\n";
            std::cout << "\tCV vers la solution = " << "\n";
            x0.display("x");

        }//end GC

        
        
        
        

    }// end namespace projective
} // end namespace solbe

#endif /*end __FUNCTIONS_H__ */

