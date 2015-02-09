#include "CollisionListener.h"


CollisionListener::CollisionListener(){
	b2ContactListener::b2ContactListener();
}


void CollisionListener::BeginContact(b2Contact* contact) {

}


void CollisionListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)  {



}