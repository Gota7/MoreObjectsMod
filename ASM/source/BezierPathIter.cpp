#include "SM64DS_2.h"

void InterpCubic(Vector3* n, Vector3* a, Vector3* b, Vector3* c, Vector3* d, Fix12i currTime) {
    
    Fix12i iVar1;
    Fix12i iVar2;
    Fix12i iVar3;
    Fix12i iVar4;
    Fix12i iVar5;
    Fix12i iVar6;
    Fix12i iVar7;
  
    iVar3 = b->y;
    iVar5 = a->y;
    iVar7 = b->z;
    iVar1 = c->y;
    iVar6 = a->z;
    iVar4 = c->z;
    n->x = (b->x * 3 - a->x) + c->x * -3 + d->x;
    iVar2 = d->z;

    n->y = (iVar3 * 3 - iVar5) + iVar1 * -3 + d->y;
    n->z = (iVar7 * 3 - iVar6) + iVar4 * -3 + iVar2;

    *n *= currTime;

    iVar6 = a->y;
    iVar5 = b->y;
    iVar3 = a->z;
    iVar1 = b->z;
    iVar2 = c->y;
    iVar4 = c->z;

    n->x = n->x + a->x * 3 + b->x * -6 + c->x * 3;
    n->y = n->y + iVar6 * 3 + iVar5 * -6 + iVar2 * 3;
    n->z = n->z + iVar3 * 3 + iVar1 * -6 + iVar4 * 3;

    *n *= currTime;

    iVar2 = a->y;
    iVar4 = b->y;
    iVar1 = b->z;
    iVar3 = a->z;

    n->x = n->x + a->x * -3 + b->x * 3;
    n->y = n->y + iVar2 * -3 + iVar4 * 3;
    n->z = n->z + iVar3 * -3 + iVar1 * 3;

    *n *= currTime;
    *n += *a;

}

bool BezierPathIter::Advance() {

	Vector3 a, b, c, d, n;

    while(true){
    
        do{
        
            pathPtr.GetPt(a, currSplineX3);
            pathPtr.GetPt(b, currSplineX3 + 1);
            pathPtr.GetPt(c, currSplineX3 + 2);
            pathPtr.GetPt(d, currSplineX3 + 3);
            
            //Identical replacement function.
            InterpCubic(&n, &a, &b, &c, &d, currTime);
            
            Fix12i dist = Vec3_Dist(&n, &pos);
            
            if(step <= dist){
                
                //Fix12i result = FX_Div(step, dist);
                Fix12i result = step / dist; //Proper division replacement.

                //Func_02090DD0(&pos, &n, &pos, 0x1000 - result);

                //Replacement (just linear interpolation).
                pos -= n;
                pos *= 0x1000_f - result;
                pos += n;
                
                return true;
                
            }
                        
            if(0xFFF_f < currTime){
                                
                if(pathPtr.path->numNodes <= currSplineX3 + 6){
                    pathPtr.GetPt(pos, currSplineX3 + 3);
                    return false;
                }
                
            }
            
            currTime += tinyStep;
    
        }while(currTime <= 0xFFF_f || pathPtr.path->numNodes <= (currSplineX3 + 6));
        
        do{
        
            currTime -= 0x1000_f;
            currSplineX3 += 3;
            
            if(currTime <= 0xFFF_f){
                break;
            }
        
        }while((currSplineX3 + 6) < pathPtr.path->numNodes);
        
    }

}