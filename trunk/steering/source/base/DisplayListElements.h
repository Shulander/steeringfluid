#ifndef __DISPLAYLISTELEMENTS_H__
#define __DISPLAYLISTELEMENTS_H__

class DisplayListElements {

public:
	DisplayListElements (void);

	void keyboard (unsigned char key, int x, int y);

	void mouse(int button, int state, int x, int y);
};
#endif