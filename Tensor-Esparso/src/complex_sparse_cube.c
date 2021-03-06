#include <stdlib.h>
#include <complex_sparse_cube.h>
#include <defines.h>
#include <stdio.h>

COMPLEX_SPARSE_PLANE *complex_sparse_plane_create(long int x_dim, long int y_dim) {
	COMPLEX_SPARSE_PLANE *plane = (COMPLEX_SPARSE_PLANE *)
		malloc(sizeof(COMPLEX_SPARSE_PLANE));
	plane->x_dim = x_dim;
	plane->y_dim = y_dim;
	plane->x_index = (COMPLEX_SPARSE_PLANE_ELEM **)
		calloc(x_dim, sizeof(COMPLEX_SPARSE_PLANE_ELEM*));
	plane->y_index = (COMPLEX_SPARSE_PLANE_ELEM **)
		calloc(y_dim, sizeof(COMPLEX_SPARSE_PLANE_ELEM*));
	return plane;
}

COMPLEX_SPARSE_CUBE *complex_sparse_cube_create(
		long int d1_dim, long int d2_dim, long int d3_dim,
		double constant) {
	COMPLEX_SPARSE_CUBE *cube = (COMPLEX_SPARSE_CUBE *) 
		malloc(sizeof(COMPLEX_SPARSE_CUBE));
	cube->constant = constant;
	cube->d1_d2 = complex_sparse_plane_create(d1_dim, d2_dim);
	cube->d2_d3 = complex_sparse_plane_create(d2_dim, d3_dim);
	cube->d3_d1 = complex_sparse_plane_create(d3_dim, d1_dim);

	return cube;
}

COMPLEX_SPARSE_PLANE_ELEM *
	complex_sparse_plane_put(COMPLEX_SPARSE_PLANE *plane,
				long int x, long int y, int *error) {
	if (!plane) { *error = INVALID_PLANE; return NULL; }
	if (x < 0 || x >= plane->x_dim ||
		y < 0 || y >= plane->y_dim) {
		*error = INVALID_POS;
		return NULL;
	}
	
	COMPLEX_SPARSE_PLANE_ELEM **xp = &plane->x_index[x];
	COMPLEX_SPARSE_PLANE_ELEM **yp = &plane->y_index[y];

	while (*xp && (*xp)->y < y && (xp = &(*xp)->y_next));
	while (*yp && (*yp)->x < x && (yp = &(*yp)->x_next));

	if (*xp && *yp && (*xp)->y == y && (*yp)->x == x)
		return *xp;
	else {
		COMPLEX_SPARSE_PLANE_ELEM *new =
		   (COMPLEX_SPARSE_PLANE_ELEM *)
		      calloc(1, sizeof(COMPLEX_SPARSE_PLANE_ELEM));
		new->x = x;
		new->y = y;
		new->x_next = *yp;
		new->y_next = *xp;
		*xp = new;
		*yp = new;

		return new;
	}
}

long double complex_sparse_cube_get(COMPLEX_SPARSE_CUBE *cube,
			long int d1, long int d2, long int d3, int *error){
	if (!cube) { return INVALID_CUBE;}
	if (d1 < 0 || d1 >= cube->d1_d2->x_dim ||
		d2 < 0 || d2 >= cube->d2_d3->x_dim ||
		d3 < 0 || d3 >= cube->d3_d1->x_dim){
			return INVALID_POS;
	}

	//Getting planes
	COMPLEX_SPARSE_PLANE_ELEM *elem_d1_d2 =
		complex_sparse_plane_put(cube->d1_d2, d1, d2, error);
	COMPLEX_SPARSE_PLANE_ELEM *elem_d2_d3 =
		complex_sparse_plane_put(cube->d2_d3, d2, d3, error);
	COMPLEX_SPARSE_PLANE_ELEM *elem_d3_d1 =
		complex_sparse_plane_put(cube->d3_d1, d3, d1, error);
	
	//Getting element
	COMPLEX_SPARSE_CUBE_ELEM **p_d1_d2 = 
		&elem_d1_d2->three_d_element;
	COMPLEX_SPARSE_CUBE_ELEM **p_d2_d3 = 
		&elem_d2_d3->three_d_element;
	COMPLEX_SPARSE_CUBE_ELEM **p_d3_d1 = 
		&elem_d3_d1->three_d_element;

	while (*p_d1_d2 && (*p_d1_d2)->d3 < d3) 
		p_d1_d2 = &(*p_d1_d2)->d3_next;
	while (*p_d2_d3 && (*p_d2_d3)->d1 < d1)
		p_d2_d3 = &(*p_d2_d3)->d1_next;
	while (*p_d3_d1 && (*p_d3_d1)->d2 < d2)
		p_d3_d1 = &(*p_d3_d1)->d2_next;

	if (*p_d1_d2 && *p_d2_d3 && *p_d3_d1 &&
		*p_d1_d2 == *p_d2_d3 && *p_d2_d3 == *p_d3_d1) {
		*error = SUCCESS;
		return (*p_d1_d2)->elem;
	}
	else{
		*error = NOT_FOUND;
		return cube->constant;
	}
}

int complex_sparse_cube_remove(COMPLEX_SPARSE_CUBE *cube,
				long int d1, long int d2, long int d3, int *error){
	if (!cube) return INVALID_CUBE;
	if (d1 < 0 || d1 >= cube->d1_d2->x_dim ||
		d2 < 0 || d2 >= cube->d2_d3->x_dim ||
		d3 < 0 || d3 >= cube->d3_d1->x_dim){
			return INVALID_POS;
	}

	//Getting planes
	COMPLEX_SPARSE_PLANE_ELEM *elem_d1_d2 =
		complex_sparse_plane_put(cube->d1_d2, d1, d2, error);
	COMPLEX_SPARSE_PLANE_ELEM *elem_d2_d3 =
		complex_sparse_plane_put(cube->d2_d3, d2, d3, error);
	COMPLEX_SPARSE_PLANE_ELEM *elem_d3_d1 =
		complex_sparse_plane_put(cube->d3_d1, d3, d1, error);
	
	//Getting element
	COMPLEX_SPARSE_CUBE_ELEM **p_d1_d2 = 
		&elem_d1_d2->three_d_element;
	COMPLEX_SPARSE_CUBE_ELEM **p_d2_d3 = 
		&elem_d2_d3->three_d_element;
	COMPLEX_SPARSE_CUBE_ELEM **p_d3_d1 = 
		&elem_d3_d1->three_d_element;

	while (*p_d1_d2 && (*p_d1_d2)->d3 < d3) 
		p_d1_d2 = &(*p_d1_d2)->d3_next;
	while (*p_d2_d3 && (*p_d2_d3)->d1 < d1)
		p_d2_d3 = &(*p_d2_d3)->d1_next;
	while (*p_d3_d1 && (*p_d3_d1)->d2 < d2)
		p_d3_d1 = &(*p_d3_d1)->d2_next;

	if(*p_d1_d2 && *p_d2_d3 && *p_d3_d1 && 
		(*p_d1_d2)->d3 == d3 && (*p_d2_d3)->d1 == d1 && (*p_d3_d1)->d2 == d2){
			COMPLEX_SPARSE_CUBE_ELEM *aux = *p_d1_d2;
			*p_d1_d2 = (*p_d1_d2)->d3_next;
			*p_d2_d3 = (*p_d2_d3)->d1_next;
			*p_d3_d1 = (*p_d3_d1)->d2_next;
			free(aux);
	}

	return SUCCESS;
}

int complex_sparse_cube_put(COMPLEX_SPARSE_CUBE *cube,
				long int d1, long int d2, long int d3,
				long double elem, int *error) {
	if (!cube) return INVALID_CUBE;
	if (d1 < 0 || d1 >= cube->d1_d2->x_dim ||
		d2 < 0 || d2 >= cube->d2_d3->x_dim ||
		d3 < 0 || d3 >= cube->d3_d1->x_dim){
			return INVALID_POS;
	}
	if (cube->constant == elem) {
		return complex_sparse_cube_remove(cube, d1, d2, d3, error);
	}

	// Creating the planes
	COMPLEX_SPARSE_PLANE_ELEM *elem_d1_d2 =
		complex_sparse_plane_put(cube->d1_d2, d1, d2, error);
	COMPLEX_SPARSE_PLANE_ELEM *elem_d2_d3 =
		complex_sparse_plane_put(cube->d2_d3, d2, d3, error);
	COMPLEX_SPARSE_PLANE_ELEM *elem_d3_d1 =
		complex_sparse_plane_put(cube->d3_d1, d3, d1, error);

	// Inserting element
	COMPLEX_SPARSE_CUBE_ELEM **p_d1_d2 = 
		&elem_d1_d2->three_d_element;
	COMPLEX_SPARSE_CUBE_ELEM **p_d2_d3 = 
		&elem_d2_d3->three_d_element;
	COMPLEX_SPARSE_CUBE_ELEM **p_d3_d1 = 
		&elem_d3_d1->three_d_element;

	while (*p_d1_d2 && (*p_d1_d2)->d3 < d3) 
		p_d1_d2 = &(*p_d1_d2)->d3_next;
	while (*p_d2_d3 && (*p_d2_d3)->d1 < d1)
		p_d2_d3 = &(*p_d2_d3)->d1_next;
	while (*p_d3_d1 && (*p_d3_d1)->d2 < d2)
		p_d3_d1 = &(*p_d3_d1)->d2_next;

	if (*p_d1_d2 && *p_d2_d3 && *p_d3_d1 &&
		*p_d1_d2 == *p_d2_d3 && *p_d2_d3 == *p_d3_d1) {
		(*p_d1_d2)->elem = elem;
	} else {
		COMPLEX_SPARSE_CUBE_ELEM *new =
		   (COMPLEX_SPARSE_CUBE_ELEM *)
			calloc(1, sizeof(COMPLEX_SPARSE_CUBE_ELEM));
		new->d1 = d1;
		new->d2 = d2;
		new->d3 = d3;
		new->elem = elem;
		new->d1_next = *p_d2_d3;
		new->d2_next = *p_d3_d1;
		new->d3_next = *p_d1_d2;
		*p_d2_d3 = *p_d3_d1 = *p_d1_d2 = new;
	}
		
	return SUCCESS;
}

//Codigo nao funciona, problemas de double free
int complex_sparse_cube_free(COMPLEX_SPARSE_CUBE *cube){
	if(!cube) return INVALID_CUBE;
	int i;
	//Freeing d1_d2
	for(i = 0; i < cube->d1_d2->x_dim; i++){
		free(cube->d1_d2->x_index[i]);
	}
	free(cube->d1_d2->x_index);
	for(i = 0; i < cube->d1_d2->y_dim; i++){
		if(cube->d1_d2->y_index[i] != NULL) free(cube->d1_d2->y_index[i]);
	}
	free(cube->d1_d2->y_index);
	free(cube->d1_d2);

	//Freeing d2_d3
	for(i = 0; i < cube->d2_d3->x_dim; i++){
		free(cube->d2_d3->x_index[i]);
	}
	free(cube->d2_d3->x_index);
	for(i = 0; i < cube->d2_d3->y_dim; i++){
		if(cube->d2_d3->y_index[i] != NULL) free(cube->d2_d3->y_index[i]);
	}
	free(cube->d2_d3->y_index);
	free(cube->d2_d3);

	//Freeing d3_d1
	for(i = 0; i < cube->d3_d1->x_dim; i++){
		free(cube->d3_d1->x_index[i]);
	}
	free(cube->d3_d1->x_index);
	for(i = 0; i < cube->d3_d1->y_dim; i++){
		if(cube->d3_d1->y_index[i] != NULL) free(cube->d3_d1->y_index[i]);
	}
	free(cube->d3_d1->y_index);
	free(cube->d3_d1);

	//Freeing cube
	free(cube);
	return SUCCESS;
}

