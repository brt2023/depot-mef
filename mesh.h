#ifndef __MESH_H__
#define __MESH_H__

#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <limits>
#include <cassert>

typedef double dble;






/********** class Label **********/
/*
 * Reference des numeros de triangle, de sommet
 */
class Label{
public:
    int num;
    Label(int nnum=0):num(nnum){};
    
    friend std::ostream& operator<<(std::ostream& f, const Label& lab){
        f << lab.num;
        return f;        
    }
    friend std::istream& operator>>(std::istream& f, Label& lab){
        f >> lab.num;
        return f;
    }

};/*END class Label */





/********** class R2 **********/

class R2{
public:    
    dble x,y;
    R2(/**/):x(0.0),y(0.0){}
    R2(dble xx,dble yy):x(xx),y(yy){}
    R2(const R2&p) = default;//copy constructor
    R2(R2 p1,R2 p2):x(p2.x-p1.x),y(p2.y-p1.y){}
    R2& operator=(const R2&p) = default;//copy by assignment
    
    R2  operator+ (const R2&p);
    R2  operator- (const R2&p);
    R2  operator* (const R2&p);
    R2& operator+=(const R2&p);
    R2& operator-=(const R2&p);
    R2  operator* (const dble& c);
    dble operator,(const R2&p);//dot product
    dble operator^(const R2&p);//vector product
    
    
    
    friend std::ostream& operator<<(std::ostream& f, const R2& p){
        f << p.x << " " << p.y;
        return f;
    }
    friend std::istream& operator>>(std::istream& f, R2& p){
        f >> p.x >> p.y;
        return f;
    }    
    void print(){
        std::cout << "(" << x << "," << y << ")" << std::endl;
    }
  
};/*END class R2 */

R2 R2::operator*(const dble& c)
{
    return R2(c*x,c*y);
}


dble R2::operator^(const R2& p)
{
    return (x*p.y - y*p.x);
}
dble R2::operator,(const R2& p)
{
    return (x*p.x + y*p.y);
}
R2 R2::operator*(const R2& p)
{
    return R2(x*p.x,y*p.y);
}
R2 & R2::operator+=(const R2& p)
{
    x += p.x;
    y += p.y;
    return (*this);
}
R2 & R2::operator-=(const R2& p)
{
    x -= p.x;
    y -= p.y;
    return (*this);
}
R2 R2::operator+(const R2& p)
{
    return R2(x+p.x,y+p.y);
}
R2 R2::operator-(const R2& p)
{
    return R2(x-p.x,y-p.y);
}



/********** class Vertex **********/

class Vertex: public R2,public Label{
public:
    int ns;
    int neb;
    R2 p;    // affichage du vecteur vv

    Vertex(/**/){};
    Vertex(int nns, R2 pp, int nneb):ns(nns),p(pp),neb(nneb){};
    void display();
    
    friend std::ostream& operator<<(std::ostream& f,const Vertex& v){
        f << v.ns << " " << v.p << " " << v.neb;
        return f;
    }
    friend std::istream& operator>>(std::istream& f, Vertex& v){
        f >> v.ns >> v.p >> v.neb;
        return f;
    }

};/*END class Vertex */

void Vertex::display()
{
    std::cout << ns << "," << p << "," << neb << std::endl;
}




/********** class Triangle **********/
class Triangle:public Label{
public:
    dble area;
    Vertex *vertices[3];
    
    Triangle(/**/){};
    Triangle(int numtri):Label(numtri){}
    void set(Vertex*v0,int i,int iA,int iB,int iC,int j);
    Vertex & operator[](int& i){
        assert(i>=0);assert(i<3);
        return *vertices[i];        
    }
    
};/*END class Triangle */
void Triangle::set(Vertex* v0, int i, int iA, int iB, int iC, int j)
{
    dble c = 0.5;
    vertices[0] = v0 + iA;
    vertices[1] = v0 + iB;
    vertices[2] = v0 + iC;
    num = j;

    R2 AB(vertices[0]->p , vertices[1]->p);
    R2 AC(vertices[0]->p , vertices[2]->p);
    area = AB^AC*c;
    assert(area>=0);
}







class Mesh{
public:
    int nbs,nbt;
    dble area = 0.0;

    Vertex *vertices;
    Triangle *triangle;
    Mesh(/**/){};
    Mesh(const char * filename);
    
};/*END class Mesh */


Mesh::Mesh(const char * filename)
{
    /*
     * lecture maillage
     */
    std::ifstream f(filename);
    if(!f){
        std::cerr << "\tErreur: probleme ouverture du fichier ou fichier inexistant !"<< std::endl;
        exit(1);
    }   
    f >> nbs >> nbt;
    
    /*
     * remplissage des classes
     */
    std::cout << "nbre de sommet   (nbs) = " << nbs << std::endl;
    std::cout << "nbre de triangle (nbt) = " << nbt << std::endl;
    
    // lecture des vertex...FIXME
    vertices = new Vertex[nbs];
    triangle = new Triangle[nbt];
    std::vector<Vertex>vv;
    for(int k=0;k<nbs;k++){
        f >> vertices[k];
        vv.push_back(vertices[k]);
        vertices[k].display();
    }
    
    std::cout << "\n-------------------------------------------------------------------\n";

    int nt, iA, iB, iC, nebt;
    for(int i=0;i<nbt;i++){
        f >> nt >> iA >> iB >> iC >> nebt;
        triangle[i].set(vertices, nt, iA-1, iB-1, iC-1, nebt);
        std::cout << nt << " " << iA-1 << " " << iB-1 << " " << iC-1 << " " << nebt << std::endl;
        area += triangle[i].area;
    }


}





#endif /*__MESH_H__*/
