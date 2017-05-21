#ifndef HITABLELISTH
#define HITABLELISTH

#include "Hitable.h"

class HitableList : public Hitable {
public:
	HitableList() {};
	HitableList(Hitable **l, int n) { list = l; listSize = n; };

	virtual bool Hit(const Ray& r, const float tMin, const float tMax, HitRecord& rec) const;

	Hitable **list;
	int listSize;
};

bool HitableList::Hit(const Ray& r, const float tMin, const float tMax, HitRecord& rec) const {
	HitRecord tempRec;
	bool anythingHit = false;
	float currentClosest = tMax;
	for (int i = 0; i < listSize; i++) {
		if (list[i]->Hit(r, tMin, currentClosest, tempRec)) {
			anythingHit = true;
			currentClosest = tempRec.t;
			rec = tempRec;
		}
	}
	return anythingHit;
}

#endif
