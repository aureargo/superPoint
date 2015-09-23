#ifndef CONVOLUTION_H
#define CONVOLUTION_H

std::vector<glm::vec3> convolution(const std::vector<glm::vec3>& colors, int h, int w)
{
    std::vector<glm::vec3> colors2(colors);
    for(int y = 0;  y < h;  y++)
    {
        for(int x = 0;  x < w;  x++)
        {
            int i = y * w + x;
            float div = 8;
            colors2[i] = colors[i]*div;
            if(y > 0)
            {
                if(x > 0)   {
                    colors2[i] += colors[(y-1)*w + (x-1)];
                    div++;
                }
                colors2[i] += colors[(y-1)*w + x];
                div++;
                if(x < w-1) {
                    colors2[i] += colors[(y-1)*w + (x+1)];
                }
            }
            if(x > 0)   {
                colors2[i] += colors[y*w + (x-1)];
                div++;
            }
            if(x < w-1) {
                colors2[i] += colors[y*w + (x+1)];
                div++;
            }
            if(y < h-1)
            {
                if(x > 0)   {
                    colors2[i] += colors[(y+1)*w + (x-1)];
                    div++;
                }
                colors2[i] += colors[(y+1)*w + x];
                div++;
                if(x < w-1) {
                    colors2[i] += colors[(y+1)*w + (x+1)];
                    div++;
                }
            }
            colors2[i] /= div;
        }
    }
    return colors2;
}

#endif // CONVOLUTION_H
