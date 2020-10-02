//Scaueru Eduard 313CD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_header.h"

void write(image **bmp_image, char *image_fname, int task, bmp_fileheader *file_header, bmp_infoheader *info_header)
{
    char *aux = malloc(50);
    int padding, i, j;

    strcpy(aux, image_fname);
    aux = strtok(aux, ".bmp");
    switch (task)
    {
        case 1:
            strcat(aux, "_black_white.bmp");	
            break;
        case 2:
            strcat(aux, "_nocrop.bmp");
            break;
        case 3:
            strcat(aux, "_filter.bmp");
            break;
        case 4:
            strcat(aux, "_pooling.bmp");
            break;
        case 5:
            strcat(aux, "_clustered.bmp");
            break;
    }
    FILE *outputFile = fopen(aux, "wb");
    fwrite(file_header, sizeof(bmp_fileheader), 1, outputFile);
    fwrite(info_header, sizeof(bmp_infoheader), 1, outputFile);
    padding = (4 - ((info_header->width * 3) % 4)) % 4;
    for(i = 0; i < info_header->height; i++)
    {
        for(j = 0; j < info_header->width; j++)
        {
            fwrite(&bmp_image[i][j], sizeof(image), 1, outputFile);
        }
        for(j = 0; j < padding; j++)
        {
            fputc(0, outputFile);
        }
    }
    free(aux);
    fclose(outputFile);
}

void black_white(image **bmp_image, char *image_fname, bmp_fileheader *file_header, bmp_infoheader *info_header)
{
    int i, j;
    unsigned char x;
    image **new_image = (image**)malloc(info_header->height * sizeof(image*));
    for(i = 0; i < info_header->height; i++)
    {
        new_image[i] = (image*)malloc(info_header->width * sizeof(image));
    }

    for(i = 0; i < info_header->height; i++)
    {
        for(j = 0; j < info_header->width; j++)
        {
            x = (bmp_image[i][j].r + bmp_image[i][j].g + bmp_image[i][j].b) / 3;
            new_image[i][j].r = x;
            new_image[i][j].g = x;
            new_image[i][j].b = x;
        }
    }

    write(new_image, image_fname, 1, file_header, info_header);

    for(i = 0; i < info_header->height; i++)
    {
        free(new_image[i]);
    }
    free(new_image);
}

void no_crop(image **bmp_image, char *image_fname, bmp_fileheader *file_header, bmp_infoheader *info_header)
{
    int up, down, dif, i, ii, j, oldw, oldh;
    image **new_image;

    oldh = info_header->height;
    oldw = info_header->width;
    if(info_header->width != info_header->height)
    {
        if(info_header->width > info_header->height)
        {
            dif = info_header->width - info_header->height;
            info_header->height = info_header->width;

            new_image = (image**)malloc(info_header->height * sizeof(image*));
            for(i = 0; i < info_header->height; i++)
            {
                new_image[i] = (image*)malloc(info_header->width * sizeof(image));
            }

            if(dif % 2 == 1)
            {
                up = dif / 2 + 1;
                down = dif / 2;
            }
            else
            {
                up = dif / 2;
                down = up;
            }
            for(i = 0; i < up; i++)
            {
                for(j = 0; j < info_header->width; j++)
                {
                    new_image[i][j].r = 255;
                    new_image[i][j].g = 255;
                    new_image[i][j].b = 255;
                }
            }
            ii = 0;
            for(; i < info_header->height - down; i++)
            {
                for(j = 0; j < info_header->width; j++)
                {
                    new_image[i][j].r = bmp_image[ii][j].r;
                    new_image[i][j].g = bmp_image[ii][j].g;
                    new_image[i][j].b = bmp_image[ii][j].b;
                }
                ii++;
            }
            for(; i < info_header->height; i++)
            {
                for(j = 0; j < info_header->width; j++)
                {
                    new_image[i][j].r = 255;
                    new_image[i][j].g = 255;
                    new_image[i][j].b = 255;
                }
            }
        }
        else
        {
            dif = info_header->height - info_header->width;
            info_header->width = info_header->height;

            new_image = (image**)malloc(info_header->height * sizeof(image*));
            for(i = 0; i < info_header->height; i++)
            {
                new_image[i] = (image*)malloc(info_header->width * sizeof(image));
            }

            if(dif % 2 == 1)
            {
                up = dif / 2 + 1;
                down = dif / 2;
            }
            else
            {
                up = dif / 2;
                down = up;
            }
            for(i = 0; i < up; i++)
            {
                for(j = 0; j < info_header->height; j++)
                {
                    new_image[j][i].r = 255;
                    new_image[j][i].g = 255;
                    new_image[j][i].b = 255;
                }
            }
            ii = 0;
            for(; i < info_header->width - down; i++)
            {
                for(j = 0; j < info_header->height; j++)
                {
                    new_image[j][i].r = bmp_image[j][ii].r;
                    new_image[j][i].g = bmp_image[j][ii].g;
                    new_image[j][i].b = bmp_image[j][ii].b;
                }
                ii++;
            }
            for(; i < info_header->width; i++)
            {
                for(j = 0; j < info_header->height; j++)
                {
                    new_image[j][i].r = 255;
                    new_image[j][i].g = 255;
                    new_image[j][i].b = 255;
                }
            }
        }
    }
    else
    {
        new_image = (image**)malloc(info_header->height * sizeof(image*));
        for(i = 0; i < info_header->height; i++)
        {
            new_image[i] = (image*)malloc(info_header->width * sizeof(image));
        }

        for(i = 0; i < info_header->height; i++)
        {
            for(j = 0; j < info_header->width; j++)
            {
                new_image[i][j].r = bmp_image[i][j].r;
                new_image[i][j].g = bmp_image[i][j].g;
                new_image[i][j].b = bmp_image[i][j].b;
            }
        }
    }

    write(new_image, image_fname, 2, file_header, info_header);

    for(i = 0; i < info_header->height; i++)
    {
        free(new_image[i]);
    }
    free(new_image);
    info_header->height = oldh;
    info_header->width = oldw;
}

int inMatrix(int i, int j, int lin, int col)
{
	if(i >= 0 && i < lin && j >= 0 && j < col) return 1;
	return 0;
}

void convolutional_layers(image **bmp_image, char *image_fname, bmp_fileheader *file_header, bmp_infoheader *info_header, char *filter_fname)
{
    int i, j, n, **m, ii, jj, r, g, b, lin, col;
    image **new_image = (image**)malloc(info_header->height * sizeof(image*));
    image **new_image_cpy = (image**)malloc(info_header->height * sizeof(image*));
    for(i = 0; i < info_header->height; i++)
    {
        new_image[i] = (image*)malloc(info_header->width * sizeof(image));
    }
    for(i = 0; i < info_header->height; i++)
    {
        new_image_cpy[i] = (image*)malloc(info_header->width * sizeof(image));
    }
    FILE *f = fopen(filter_fname, "rt");
    fscanf(f, "%d", &n);
    m = malloc(n * sizeof(int*));
    for (i = 0; i < n; i++)
    {
        m[i] = malloc(n * sizeof(int));
    }
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            fscanf(f, "%d", &m[i][j]);
        }
    }
    for(i = 0; i < info_header->height; i++)
    {
        for(j = 0; j < info_header->width; j++)
        {
            new_image[i][j].r = bmp_image[info_header->height - 1 - i][j].r;
            new_image[i][j].g = bmp_image[info_header->height - 1 - i][j].g;
            new_image[i][j].b = bmp_image[info_header->height - 1 - i][j].b;
        }
    }
    for(i = 0; i < info_header->height; i++)
    {
        for(j = 0; j < info_header->width; j++)
        {            
            r = 0;
            g = 0;
            b = 0;
            lin = 0;
            for(ii = i - n/2; ii <= i + n/2; ii++)
            {
                col = 0;
                for(jj = j - n/2; jj <= j + n/2; jj++)
                {
                	if(inMatrix(ii, jj, info_header->height, info_header->width))
                	{
                    	r += new_image[ii][jj].r * m[lin][col];
                    	g += new_image[ii][jj].g * m[lin][col];
                    	b += new_image[ii][jj].b * m[lin][col];
                    }
                    col++;
                }
                lin++;
            }
            if(r < 0) r = 0;
            else if(r > 255) r = 255;
            if(g < 0) g = 0;
            else if(g > 255) g = 255;
            if(b < 0) b = 0;
            else if(b > 255) b = 255;
            new_image_cpy[info_header->height - 1 - i][j].r = r;
            new_image_cpy[info_header->height - 1 - i][j].g = g;
            new_image_cpy[info_header->height - 1 - i][j].b = b;
        }
    }

    write(new_image_cpy, image_fname, 3, file_header, info_header);

    for(i = 0; i < n; i++)
    {
        free(m[i]);
    }
    for(i = 0; i < info_header->height; i++)
    {
        free(new_image[i]);
    }
    free(new_image);
    for(i = 0; i < info_header->height; i++)
    {
        free(new_image_cpy[i]);
    }
    free(new_image_cpy);
    free(m);
    fclose(f);
}

void poolin_layer(image **bmp_image, char *image_fname, bmp_fileheader *file_header, bmp_infoheader *info_header, char *pooling_fname)
{
	int i, j, n, r_min, g_min, b_min, r_max, g_max, b_max, ii, jj;
	char c;
	image **new_image = (image**)malloc(info_header->height * sizeof(image*));
    image **new_image_cpy = (image**)malloc(info_header->height * sizeof(image*));
    for(i = 0; i < info_header->height; i++)
    {
        new_image[i] = (image*)malloc(info_header->width * sizeof(image));
    }
    for(i = 0; i < info_header->height; i++)
    {
        new_image_cpy[i] = (image*)malloc(info_header->width * sizeof(image));
    }
    FILE *f = fopen(pooling_fname, "rt");
    fscanf(f, "%c %d", &c, &n);

   	for(i = 0; i < info_header->height; i++)
    {
        for(j = 0; j < info_header->width; j++)
        {
            new_image[i][j].r = bmp_image[info_header->height - 1 - i][j].r;
            new_image[i][j].g = bmp_image[info_header->height - 1 - i][j].g;
            new_image[i][j].b = bmp_image[info_header->height - 1 - i][j].b;
        }
    }
    for(i = 0; i < info_header->height; i++)
    {
        for(j = 0; j < info_header->width; j++)
        {
        	r_max = 0;
        	g_max = 0;
        	b_max = 0;
        	r_min = 255;
        	g_min = 255;
        	b_min = 255;
        	for(ii = i - n/2; ii <= i + n/2; ii++)
            {
                for(jj = j - n/2; jj <= j + n/2; jj++)
                {
                	if(inMatrix(ii, jj, info_header->height, info_header->width))
                	{
                    	if(new_image[ii][jj].r >= r_max) r_max = new_image[ii][jj].r;
                    	if(new_image[ii][jj].r <= r_min) r_min = new_image[ii][jj].r;
                    	if(new_image[ii][jj].g >= g_max) g_max = new_image[ii][jj].g;
                    	if(new_image[ii][jj].g <= g_min) g_min = new_image[ii][jj].g;
                    	if(new_image[ii][jj].b >= b_max) b_max = new_image[ii][jj].b;
                    	if(new_image[ii][jj].b <= b_min) b_min = new_image[ii][jj].b;
                    }
                    else
                    {
                    	r_min = 0;
			        	g_min = 0;
			        	b_min = 0;
                    }
                }
            }
            if(c == 'm')
            {
            	new_image_cpy[info_header->height - 1 - i][j].r = r_min;
            	new_image_cpy[info_header->height - 1 - i][j].g = g_min;
            	new_image_cpy[info_header->height - 1 - i][j].b = b_min;
            }
            else
            {
            	new_image_cpy[info_header->height - 1 - i][j].r = r_max;
            	new_image_cpy[info_header->height - 1 - i][j].g = g_max;
            	new_image_cpy[info_header->height - 1 - i][j].b = b_max;
            }
        }
    }

    write(new_image_cpy, image_fname, 4, file_header, info_header);

    for(i = 0; i < info_header->height; i++)
    {
        free(new_image[i]);
    }
    free(new_image);
    for(i = 0; i < info_header->height; i++)
    {
        free(new_image_cpy[i]);
    }
    free(new_image_cpy);
    fclose(f);
}

void clustering(image **bmp_image, char *image_fname, bmp_fileheader *file_header, bmp_infoheader *info_header, char *cluster_fname)
{
	int i, j, threshold, **m, ii, jj, pr, ul, k, x1, y1, ok, nr_pix, r, g, b, nr;
	typedef struct
    {
    	int lin, col;
    }op;
    op *coada = calloc(1000005, sizeof(op));
    op *v = calloc(1000005, sizeof(op));
    int dl[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};
    m = (int**)malloc(info_header->height * sizeof(int*));
	for(i = 0; i < info_header->height; i++)
	{
		m[i] = (int*)calloc(info_header->width, sizeof(int));
	}
	image **new_image = (image**)malloc(info_header->height * sizeof(image*));
    image **new_image_cpy = (image**)malloc(info_header->height * sizeof(image*));
    for(i = 0; i < info_header->height; i++)
    {
        new_image[i] = (image*)malloc(info_header->width * sizeof(image));
    }
    for(i = 0; i < info_header->height; i++)
    {
        new_image_cpy[i] = (image*)malloc(info_header->width * sizeof(image));
    }
    FILE *f = fopen(cluster_fname, "rt");
    fscanf(f, "%d", &threshold);
    for(i = 0; i < info_header->height; i++)
    {
        for(j = 0; j < info_header->width; j++)
        {
            new_image[i][j].r = bmp_image[info_header->height - 1 - i][j].r;
            new_image[i][j].g = bmp_image[info_header->height - 1 - i][j].g;
            new_image[i][j].b = bmp_image[info_header->height - 1 - i][j].b;
        }
    }
    ok = 1;
    while(ok)
    {
    	ok = 0;
	    for(i = 0; i < info_header->height; i++)
	    {
	    	for(j = 0; j < info_header->width; j++)
	    	{
	    		if(m[i][j] == 0)
	    		{
	    			x1 = i;
	    			y1 = j;
	    			ok = 1;
	    			break;
	    		}
	    	}
	    	if(ok == 1) break;
	    }
	    if(ok == 0) break;
	    m[x1][y1] = -1;
	    pr = 1;
	    ul = 1;
	    coada[1].lin = x1;
	    coada[1].col = y1;
	    nr = 1;
	    nr_pix = 0;
	    v[nr_pix].lin = x1;
	    v[nr_pix].col = y1;
	    r = new_image[x1][y1].r;
	    g = new_image[x1][y1].g;
	    b = new_image[x1][y1].b;
	    while(nr > 0)
	    {
	    	i = coada[pr].lin;
	    	j = coada[pr].col;
	    	for(k = 0; k < 4; k++)
	    	{
	    		ii = i + dl[k];
	    		jj = j + dc[k];
	    		if(inMatrix(ii, jj, info_header->height, info_header->width) && m[ii][jj] == 0 && 
	    			((abs(new_image[x1][y1].r - new_image[ii][jj].r) + abs(new_image[x1][y1].g - new_image[ii][jj].g) + abs(new_image[x1][y1].b - new_image[ii][jj].b)) <= threshold))
	    		{
	    			nr_pix++;
	    			v[nr_pix].lin = ii;
	    			v[nr_pix].col = jj;
	    			r += new_image[ii][jj].r;
	    			g += new_image[ii][jj].g;
	    			b += new_image[ii][jj].b;
	    			m[ii][jj] = -1;
	    			ul++;
	    			coada[ul].lin = ii;
	    			coada[ul].col = jj;
	    			nr++;
	    		}
	    	}
	    	pr++;
	    	nr--;
	    }
	    nr_pix++;
	    if(nr_pix > 1)
	    {
	    	for(i = 0; i < nr_pix; i++)
	   	 	{
	    		new_image_cpy[info_header->height - v[i].lin - 1][v[i].col].r = r / nr_pix;
	    		new_image_cpy[info_header->height - v[i].lin - 1][v[i].col].g = g / nr_pix;
	    		new_image_cpy[info_header->height - v[i].lin - 1][v[i].col].b = b / nr_pix;
	    	}
	    }
	    else
	    {
	    	new_image_cpy[info_header->height - x1 - 1][y1].r = new_image[x1][y1].r;
	    	new_image_cpy[info_header->height - x1 - 1][y1].g = new_image[x1][y1].g;
	    	new_image_cpy[info_header->height - x1 - 1][y1].b = new_image[x1][y1].b;
	    }
	}

	write(new_image_cpy, image_fname, 5, file_header, info_header);

	for(i = 0; i < info_header->height; i++)
    {
        free(new_image[i]);
        free(m[i]);
        free(new_image_cpy[i]);
    }
    free(new_image);
    free(m);
    free(new_image_cpy);
    free(coada);
    free(v);
    fclose(f);
}

int main()
{
    int i, j, padding;
    char *image_fname = malloc(50), *filter_fname = malloc(50), *pooling_fname = malloc(50), *cluster_fname = malloc(50);
    image **bmp_image;
    FILE *f_text = fopen("input.txt", "rt");
    fscanf(f_text, "%s", image_fname);
    fscanf(f_text, "%s", filter_fname);
    fscanf(f_text, "%s", pooling_fname);
    fscanf(f_text, "%s", cluster_fname);
    fclose(f_text);
    FILE *f_bmp = fopen(image_fname, "rwb");
    bmp_fileheader *file_header = (bmp_fileheader*)malloc(sizeof(bmp_fileheader));
    bmp_infoheader *info_header = (bmp_infoheader*)malloc(sizeof(bmp_infoheader));

    fread(file_header, sizeof(bmp_fileheader), 1, f_bmp);
    fread(info_header, sizeof(bmp_infoheader), 1, f_bmp);

    bmp_image = (image**)malloc(info_header->height * sizeof(image*));
    for(i = 0; i < info_header->height; i++)
    {
        bmp_image[i] = (image*)malloc(info_header->width * sizeof(image));
    }

    padding = (4 - ((info_header->width * 3) % 4)) % 4;
    for(i = 0; i < info_header->height; i++)
    {
        for(j = 0; j < info_header->width; j++)
        {
            fread(&bmp_image[i][j], sizeof(image), 1, f_bmp);
        }
        fseek(f_bmp, padding, SEEK_CUR);
    }

    black_white(bmp_image, image_fname, file_header, info_header);
    no_crop(bmp_image, image_fname, file_header, info_header);
    convolutional_layers(bmp_image, image_fname, file_header, info_header, filter_fname);
    poolin_layer(bmp_image, image_fname, file_header, info_header, pooling_fname);
    clustering(bmp_image, image_fname, file_header, info_header, cluster_fname);

    for(i = 0; i < info_header->height; i++)
    {
        free(bmp_image[i]);
    }
    free(bmp_image);
    fclose(f_bmp);
    free(image_fname);
    free(filter_fname);
    free(pooling_fname);
    free(cluster_fname);
    free(file_header);
    free(info_header);
    return 0;
}