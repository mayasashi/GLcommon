//
//  main.cpp
//  continuousDeformation_project
//
//  Created by 鶴拳 on 2018/04/04.
//  Copyright © 2018年 mayasashi. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[]) {
	const int *a;
	int b = 10;
	a = &b;
	printf("%d\n", *a);
	*(&b) = 20;
	printf("%d\n", *a);

    return 0;
}
