#ifndef IMU3D_h
#define IMU3D_h

#include "headers/commons.h"

namespace IMU3DViz
{
  bool is_init = false;

  offset_t offset;

  double r_rand = PI / 180;

  double r_alpha = 19.47 * PI / 180;
  double r_gamma = 20.7 * PI / 180;

  double sin_alpha = sin(19.47 * PI / 180);
  double cos_alpha = cos(19.47 * PI / 180);
  double sin_gamma = sin(20.7 * PI / 180);
  double cos_gamma = cos(20.7 * PI / 180);

  m5::IMU_Class *imu;

  float accX = 0;
  float accY = 0;
  float accZ = 0;

  double theta = 0, last_theta = 0;
  double phi = 0, last_phi = 0;
  double alpha = 0.2;

  line_3d_t ax_orig_x;
  line_3d_t ax_orig_y;
  line_3d_t ax_orig_z;

  line_3d_t rect_source[12];
  line_3d_t rect_dis;

  float cube_size = 15;
  Point3D origin = {80, 0, 0};

  line_3d_t rect[12] = {
      {.start_point = {-1, -1, 1}, .end_point = {1, -1, 1}},
      {.start_point = {1, -1, 1}, .end_point = {1, 1, 1}},
      {.start_point = {1, 1, 1}, .end_point = {-1, 1, 1}},
      {.start_point = {-1, 1, 1}, .end_point = {-1, -1, 1}},
      {
          .start_point = {-1, -1, 1},
          .end_point = {-1, -1, -1},
      },
      {
          .start_point = {1, -1, 1},
          .end_point = {1, -1, -1},
      },
      {
          .start_point = {1, 1, 1},
          .end_point = {1, 1, -1},
      },
      {
          .start_point = {-1, 1, 1},
          .end_point = {-1, 1, -1},
      },
      {.start_point = {-1, -1, -1}, .end_point = {1, -1, -1}},
      {.start_point = {1, -1, -1}, .end_point = {1, 1, -1}},
      {.start_point = {1, 1, -1}, .end_point = {-1, 1, -1}},
      {.start_point = {-1, 1, -1}, .end_point = {-1, -1, -1}},
  };

  bool point3Dto2D(Point3D *source, Point2D *point)
  {
    point->x = (source->x * cos_gamma) - (source->y * sin_gamma);
    point->y = -(source->x * sin_gamma * sin_alpha) -
               (source->y * cos_gamma * sin_alpha) + (source->z * cos_alpha);
    return true;
  }

  bool point2DToDisPoint(Point2D *point, uint8_t *x, uint8_t *y)
  {
    *x = point->x + 120;
    *y = 67 - point->y;
    return true;
  }

  bool printLine3D(LGFX_Sprite *screen_buf, line_3d_t *line, uint32_t color)
  {
    uint8_t start_x, start_y, end_x, end_y;
    Point2D point;
    point3Dto2D(&line->start_point, &point);
    point.x += origin.x;
    point.y += origin.y;
    point2DToDisPoint(&point, &start_x, &start_y);
    point3Dto2D(&line->end_point, &point);
    point.x += origin.x;
    point.y += origin.y;
    point2DToDisPoint(&point, &end_x, &end_y);

    screen_buf->drawLine(start_x, start_y, end_x, end_y, (int)color);

    return true;
  }

  void RotatePoint(Point3D *point, double x, double y, double z)
  {
    if (x != 0)
    {
      point->y = point->y * cos(x * r_rand) - point->z * sin(x * r_rand);
      point->z = point->y * sin(x * r_rand) + point->z * cos(x * r_rand);
    }

    if (y != 0)
    {
      point->x = point->z * sin(y * r_rand) + point->x * cos(y * r_rand);
      point->z = point->z * cos(y * r_rand) - point->x * sin(y * r_rand);
    }

    if (z != 0)
    {
      point->x = point->x * cos(z * r_rand) - point->y * sin(z * r_rand);
      point->y = point->x * sin(z * r_rand) + point->y * cos(z * r_rand);
    }
  }

  void RotatePoint(Point3D *point, Point3D *point_new, double x, double y,
                   double z)
  {
    if (x != 0)
    {
      point_new->y = point->y * cos(x * r_rand) - point->z * sin(x * r_rand);
      point_new->z = point->y * sin(x * r_rand) + point->z * cos(x * r_rand);
    }

    if (y != 0)
    {
      point_new->x = point->z * sin(y * r_rand) + point->x * cos(y * r_rand);
      point_new->z = point->z * cos(y * r_rand) - point->x * sin(y * r_rand);
    }

    if (z != 0)
    {
      point_new->x = point->x * cos(z * r_rand) - point->y * sin(z * r_rand);
      point_new->y = point->x * sin(z * r_rand) + point->y * cos(z * r_rand);
    }
  }

  LGFX_Sprite *screen_buf;
  void init_cube()
  {
    screen_buf = ExerSense::screen_buf;
    imu = &StickCP2.Imu;
    offset.start = {.x = 10, .y = 10};
    offset.end = {.x = (double) screen_buf->width() - 10, .y = (double) screen_buf->height() - 10};
    ax_orig_x = {.start_point = {0, 0, 0}, .end_point = {0, 0, 0}};
    ax_orig_y = {.start_point = {0, 0, 0}, .end_point = {0, 0, 0}};
    ax_orig_z = {.start_point = {0, 0, 0}, .end_point = {0, 0, cube_size}};
    for (int n = 0; n < 12; n++)
    {
      rect_source[n].start_point.x = rect[n].start_point.x * cube_size;
      rect_source[n].start_point.y = rect[n].start_point.y * cube_size;
      rect_source[n].start_point.z = rect[n].start_point.z * cube_size;
      rect_source[n].end_point.x = rect[n].end_point.x * cube_size;
      rect_source[n].end_point.y = rect[n].end_point.y * cube_size;
      rect_source[n].end_point.z = rect[n].end_point.z * cube_size;
    }
    is_init = true;
  }

  void draw_axes()
  {
    ax_orig_z.end_point.x = 0;             // axis lines origin
    ax_orig_z.end_point.y = 0;             // axis lines origin
    ax_orig_z.end_point.z = cube_size * 2; // axis lines length
    // We only rotate the z axes and re-calculate x and y as 90 degree rotations from it, saving computational power
    RotatePoint(&ax_orig_z.end_point, theta, phi, 0);
    RotatePoint(&ax_orig_z.end_point, &ax_orig_x.end_point, -90, 0, 0);
    RotatePoint(&ax_orig_z.end_point, &ax_orig_y.end_point, 0, 90, 0);
    printLine3D(screen_buf, &ax_orig_x, TFT_RED);
    printLine3D(screen_buf, &ax_orig_y, TFT_GREEN);
    printLine3D(screen_buf, &ax_orig_z, TFT_BLUE);
  }

  void update_rotation()
  {
    imu->getAccelData(&accX, &accY, &accZ);
    printf("%f \t %f \t %f\n", accX, accY, accZ);

    // M5.MPU6886.getAccelData(&accX, &accY, &accZ);
    if ((accX < 1) && (accX > -1))
    {
      theta = asin(-accX) * 57.295;
    }
    if (accZ != 0)
    {
      phi = atan(accY / accZ) * 57.295;
    }

    theta = alpha * theta + (1 - alpha) * last_theta;
    phi = alpha * phi + (1 - alpha) * last_phi;
  }

  void draw_text()
  {
    screen_buf->setTextColor(TFT_LIGHTGRAY);
    screen_buf->setCursor(offset.end.x - 30, offset.end.y - 10);
    screen_buf->printf("%.2f", theta);
    screen_buf->setCursor(offset.end.x - 30, offset.end.y);
    screen_buf->printf("%.2f", phi);

    // Debug screen size print
    // screen_buf->setCursor(offset.start.x + 10, offset.start.y);
    // screen_buf->printf("%d, %d ", screen_buf->width(), screen_buf->height());
  }

  void draw_3d_cube()
  {
    if (!is_init)
    {
      init_cube();
    }
    update_rotation();
    draw_text();
    draw_axes();

    for (int n = 0; n < 12; n++)
    {
      RotatePoint(&rect_source[n].start_point, &rect_dis.start_point, theta, phi, (double)0);
      RotatePoint(&rect_source[n].end_point, &rect_dis.end_point, theta, phi, (double)0);
      printLine3D(screen_buf, &rect_dis, TFT_WHITE);
    }
    last_theta = theta;
    last_phi = phi;
  };

}
#endif