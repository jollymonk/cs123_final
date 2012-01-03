#include "glwidget.h"
#include "settings.h"
#include "math.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent), m_timer(this), m_fps(60.0f), m_increment(0)
{

    m_fps = FRAMES_PER_SEC;
    // Set up the camera
    m_camera.eye.x = 0.0f, m_camera.eye.y = 0.0f, m_camera.eye.z = 5.0f;
    m_camera.center.x = 0.0f, m_camera.center.y = 0.0f, m_camera.center.z = 0.0f;
    m_camera.up.x = 0.0f, m_camera.up.y = 1.0f, m_camera.up.z = 0.0f;
    m_camera.fovy = 45.0f, m_camera.near = 1.0f, m_camera.far = 1000.0f;

    // Set up 60 FPS draw loop
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));

    // Start the timer for updating the screen
    m_timer.start(1000.0f / m_fps);

    m_emitters = new Emitter*[NUM_EMITTERS];
    double width_inc = ((double) FTN_WIDTH) / ((double) NUM_EMITTERS);
    double x_pos = FTN_LEFT;

    for (int i = 0; i < NUM_EMITTERS; i++)
    {
        Emitter *e = new Emitter(x_pos);
        m_emitters[i] = e;
        x_pos += width_inc;
    }

    //initialize images
    m_image = NULL;
    m_img_zero = new QImage();
    m_img_one = new QImage();
    m_img_two = new QImage();
    m_img_three = new QImage();
    m_img_four = new QImage();
    m_img_five = new QImage();
    m_img_six = new QImage();
    m_img_seven = new QImage();
    m_img_eight = new QImage();
    m_img_nine = new QImage();
    m_img_colon = new QImage();
    m_zebra = new QImage();
    m_checkered = new QImage();
    m_fan = new QImage();
    m_concrete = new QImage();

    m_img_height = 0;
    m_img_width = 0;
    m_img_scaled_height = 0;
    m_img_scale = 0.0;
    m_curr_height = -1;

    //load all images
    loadImage(m_zebra, "/Users/mjunck/Dev/cs123/cs123_final/lab04/images/zebra.jpg");
    loadImage(m_checkered, "/Users/mjunck/Dev/cs123/cs123_final/lab04/images/check.jpg");
    loadImage(m_fan, "/Users/mjunck/Dev/cs123/cs123_final/lab04/images/fan.jpg");
    loadImage(m_concrete, "/Users/mjunck/Dev/cs123/cs123_final/lab04/images/concrete.jpg");
    loadImage(m_img_zero, "/Users/mjunck/Dev/cs123/cs123_final/lab04/images/zero.jpg");
    loadImage(m_img_one, "/Users/mjunck/Dev/cs123/cs123_final/lab04/images/one.jpg");
    loadImage(m_img_two, "/Users/mjunck/Dev/cs123/cs123_final/lab04/images/two.jpg");
    loadImage(m_img_three, "/Users/mjunck/Dev/cs123/cs123_final/lab04/images/three.jpg");
    loadImage(m_img_four, "/Users/mjunck/Dev/cs123/cs123_final/lab04/images/four.jpg");
    loadImage(m_img_five, "/Users/mjunck/Dev/cs123/cs123_final/lab04/images/five.jpg");
    loadImage(m_img_six, "/Users/mjunck/Dev/cs123/cs123_final/lab04/images/six.jpg");
    loadImage(m_img_seven, "/Users/mjunck/Dev/cs123/cs123_final/lab04/images/seven.jpg");
    loadImage(m_img_eight, "/Users/mjunck/Dev/cs123/cs123_final/lab04/images/eight.jpg");
    loadImage(m_img_nine, "/Users/mjunck/Dev/cs123/cs123_final/lab04/images/nine.jpg");

    m_num_width = m_img_zero->width();

    cout << "Finished loading all images" << endl;
}

GLWidget::~GLWidget()
{
    gluDeleteQuadric(m_quadric);
    cout << "deleted quadric " << endl;
    safeDelete(m_zebra);
    cout << "deleted zebra" << endl;
    safeDelete(m_img_zero);
    safeDelete(m_img_one);
    safeDelete(m_img_two);
    safeDelete(m_img_three);
    safeDelete(m_img_four);
    safeDelete(m_img_five);
    cout << "deleted five " << endl;
    safeDelete(m_img_six);
    safeDelete(m_img_seven);
    safeDelete(m_img_eight);
    safeDelete(m_img_nine);
    //safeDelete(m_fan);
    //safeDelete(m_concrete);
//    if (m_zebra)
//        delete m_zebra;
//    if (m_fan)
//        delete m_fan;
//    if (m_concrete)
//        delete m_concrete;
//    if (m_checkered)
//        delete m_checkered;
//    if (m_image)
//        delete m_image;
}

/**
  Called once at GL initialization time to setup OpenGL states
**/
void GLWidget::initializeGL()
{
    cout << "Initializing GL" << endl;
    // Get a new quadrics obect
    m_quadric = gluNewQuadric();

    // Enable depth testing, so that objects are occluded based on depth instead of drawing order
    glEnable(GL_DEPTH_TEST);

    // Enable back-face culling, meaning only the front side of every face is rendered
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Specify that the front face is represented by vertices in counterclockwise order (this is the default)
    glFrontFace(GL_CCW);

    // Enable color materials with ambient and diffuse lighting terms
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Set up global (ambient) lighting
    GLfloat global_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    // Set up GL_LIGHT0 with a position and lighting properties
    GLfloat ambientLight[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 2.0f, 2.0f, 2.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    cout << "Finished setting up lighting" << endl;

//    assert(m_concrete);
//    GLuint concrete_tex[1];
//    int concrete_height = m_concrete->height();
//    int concrete_width = m_concrete->width();

//    glGenTextures(1, concrete_tex);
//    glBindTexture(GL_TEXTURE_2D, concrete_tex[0]);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
//                    GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
//                    GL_NEAREST);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, concrete_width,
//                 concrete_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
//                 m_concrete);

    // Set the screen color when the color buffer is cleared (in RGBA format)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enable light 0
    glEnable(GL_LIGHT0);

    // Load the initial settings
    updateSettings();
    updateCamera();
    setPattern();

    cout << "Finished initialization " << endl;
}

/**
  The main drawing method which will be called 30 frames per second.  Here
  is where your OpenGL drawing should occur.
**/
void GLWidget::paintGL()
{
    // Clear the color and depth buffers to the current glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    paintScene();

    if (m_curr_pattern != settings.fountainPattern)
        setPattern();
    int index, col;

    //only add drops if time is current drop setting
    int time_inc = (int) m_increment % DROP_SPEED;
    if (time_inc == 0)
    //if (false)
    {
        //add new drops from image sample
        if (((settings.fountainPattern != PATTERN_CONTINUOUS) || settings.fountainPattern != PATTERN_TIME) && (m_image != NULL))
        {
            const BGRA* pix = (BGRA*) m_image->constBits();
            for (int i = 0; i < NUM_EMITTERS; i++)
            {
                Emitter *e = m_emitters[i];
                if (m_curr_height > 0)
                {
                    col = (int) (i * m_img_scale);
                    index = m_curr_height * m_img_width + col;
                    assert(col <= m_img_width);
                    assert(index <= m_img_height * m_img_width);
                    int blue = (int) pix[index].b;
                    if (blue < 50)
                    {
                        m_emitters[i]->addDrop();
                    }
                }
            }
            // 0.4 is a random constant which I found worked well to make images scale nicely
            m_curr_height -= (int) (DROP_SPEED * m_img_scale * (0.4));
            if (m_curr_height < 0)
                m_curr_height += m_img_height;

        }
        else if (m_pattern == PATTERN_TIME)
        {
            if (m_curr_hour != m_hour)
            {
                m_hour = m_curr_hour;
                int hour_ls = m_hour % 10;
                int hour_ms = m_hour / 10;
                getTimeDigit(m_hour_ls, hour_ls);
                getTimeDigit(m_hour_ms, hour_ms);
            }
            if (m_curr_min != m_min)
            {
                m_min = m_curr_min;
                int min_ls = m_min % 10;
                int min_ms = m_min / 10;
                getTimeDigit(m_min_ls, min_ls);
                getTimeDigit(m_min_ms, min_ms);
            }

            const BGRA* hour_ms_pix = (BGRA*) m_hour_ms->constBits();
            const BGRA* hour_ls_pix = (BGRA*) m_hour_ls->constBits();
            const BGRA* min_ms_pix = (BGRA*) m_min_ms->constBits();
            const BGRA* min_ls_pix = (BGRA*) m_min_ls->constBits();

            int curr_num_width = m_num_width * m_img_scale;

            //paint left hour digit
            for (int i = 0; i < curr_num_width; i++)
            {
                if (m_curr_height > 0)
                {
                    col = (int) (i * m_img_scale);
                    index = m_curr_height * m_num_width + col;
                    assert(col <= m_img_width);
                    assert(index <= m_img_height * m_img_width);
                    int blue = (int) hour_ms_pix[index].b;
                    if (blue < 50)
                        m_emitters[i]->addDrop();
                }
            }

            //paint right hour digit
            for (int i = curr_num_width; i < 2 * curr_num_width; i++)
            {
                if (m_curr_height > 0)
                {
                    col = (int) (i * m_img_scale);
                    index = m_curr_height * m_num_width + col;
                    assert(col <= m_img_width);
                    assert(index <= m_img_height * m_img_width);
                    int blue = (int) hour_ls_pix[index].b;
                    if (blue < 50)
                        m_emitters[i]->addDrop();
                }
            }

            //paint left min digit
            for (int i = 2 * curr_num_width; i < 3 * curr_num_width; i++)
            {
                if (m_curr_height > 0)
                {
                    col = (int) (i * m_img_scale);
                    index = m_curr_height * m_num_width + col;
                    assert(col <= m_img_width);
                    assert(index <= m_img_height * m_img_width);
                    int blue = (int) min_ms_pix[index].b;
                    if (blue < 50)
                        m_emitters[i]->addDrop();
                }
            }

            //paint right min digit
            for (int i = 3 * curr_num_width; i < NUM_EMITTERS; i++)
            {
                if (m_curr_height > 0)
                {
                    col = (int) (i * m_img_scale);
                    index = m_curr_height * m_num_width + col;
                    assert(col <= m_img_width);
                    assert(index <= m_img_height * m_img_width);
                    int blue = (int) min_ls_pix[index].b;
                    if (blue < 50)
                        m_emitters[i]->addDrop();
                }
            }

            // 0.4 is a random constant which I found worked well to make images scale nicely
            m_curr_height -= (int) (DROP_SPEED * m_img_scale * (0.4));
            if (m_curr_height < 0)
                m_curr_height += m_img_height;
        }

        //make continuous flow, add drops to all emitters
        else
        {
            for (int i = 0; i < NUM_EMITTERS; i++)
                m_emitters[i]->addDrop();
        }
    }

    //update drops and draw
    for (int i = 0; i < NUM_EMITTERS; i++)
    {
        Emitter *e = m_emitters[i];
        e->updateDrops();
        e->drawDroplets(m_quadric);
    }
}

void GLWidget::setPattern()
{
    m_curr_pattern = settings.fountainPattern;

    if (settings.fountainPattern == PATTERN_ZEBRA && m_zebra)
        m_image = m_zebra;
    else if (settings.fountainPattern == PATTERN_CHECKERED && m_checkered)
        m_image = m_checkered;
    else if (settings.fountainPattern == PATTERN_FAN && m_fan)
        m_image = m_fan;
    // else m_image is null and pattern is continuous
    else
        m_image = NULL;

    if (settings.fountainPattern == PATTERN_ZEBRA)
        assert(m_image == m_zebra || m_image == NULL);
    else if (settings.fountainPattern == PATTERN_CHECKERED)
        assert(m_image == m_checkered || m_image == NULL);

    if (settings.fountainPattern == PATTERN_TIME)
    {
        m_img_width = 4.0 * m_num_width;
        m_img_height = m_img_zero->height();
        m_img_scale = ((double) m_img_width / (double) NUM_EMITTERS);
        m_curr_height = m_img_height - 1;
        m_hour = 10;
        m_curr_hour = m_hour;
        m_min = 23;
        m_curr_min = m_min;
        assert(m_hour > 0 && m_hour < 13);
        assert (m_min >= 0 && m_min < 60);
        int hour_ls = m_hour % 10 ;
        int hour_ms = m_hour / 10;
        int min_ls = m_min % 10;
        int min_ms = m_min / 10;

        getTimeDigit(m_hour_ls, hour_ls);
        getTimeDigit(m_hour_ms, hour_ms);
        getTimeDigit(m_min_ls, min_ls);
        getTimeDigit(m_min_ms, min_ms);

        cout << "Finished setting time pattern " << endl;
    }
    else if (m_image)
    {
        cout << "Setting pattern " << settings.fountainPattern << endl;
        m_img_height = m_image->height();
        m_img_width = m_image->width();
        assert(m_image->height() > 0);
        assert(m_image->width() > 0);
        m_img_scale = ((double) m_image->width() / (double) NUM_EMITTERS);
        m_img_scaled_height = (int) m_img_scale * m_image->height();
        m_curr_height = m_img_height - 1;
        cout << "Finished setting pattern" << endl;
    }
    else
    {
        m_img_height = 0;
        m_img_width = 0;
        m_img_scale = 0;
        m_img_scaled_height = 0;
        m_curr_height = 0;
    }
}

/**
  Called when the screen gets resized.
  The camera is updated when the screen resizes because the aspect ratio may change.
**/
void GLWidget::resizeGL(int width, int height)
{
    // Set the viewport to fill the screen
    glViewport(0, 0, width, height);
    // Update the camera
    updateCamera();
}

/**
  Update the camera's specifications.
  You need to fill in this method.
  It gets called in resizeGL which get called automatically on intialization
  and whenever the window is resized.
**/
void GLWidget::updateCamera()
{
    float w = width();
    float h = height();
    float aspectRatio = 1.0f * w / h;
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_camera.fovy, aspectRatio, m_camera.near, m_camera.far);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_camera.eye.x, m_camera.eye.y, m_camera.eye.z,
              m_camera.center.x, m_camera.center.y, m_camera.center.z,
              m_camera.up.x, m_camera.up.y, m_camera.up.z);
}

/**
  Called when the settings are updated through the UI.
  This is already filled in for you.
**/
void GLWidget::updateSettings()
{
    switch (settings.fillMode)
    {
        case FILL_POINTS:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
        case FILL_WIREFRAME:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
        case FILL_SHADED:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    }

    if (settings.lightingEnabled)
    {
        glEnable(GL_LIGHTING);
    }
    else
    {
        glDisable(GL_LIGHTING);
    }

    switch (settings.shadingMode)
    {
        case SHADING_FLAT:
        glShadeModel(GL_FLAT);
        break;
        case SHADING_SMOOTH:
        glShadeModel(GL_SMOOTH);
        break;
    }
}


/**
  Specifies to Qt what to do when the widget needs to be updated.
  We only want to repaint the onscreen objects.
**/
void GLWidget::tick()
{
    update();
}

bool GLWidget::loadImage(const QString &file)
{
    QImage *temp = new QImage();
    if (!temp->load(file))
        return false;

    // make sure the image is RGB (not monochrome, for example)
    if (temp->format() != QImage::Format_RGB32)
    {
        QImage *old = temp;
        temp = new QImage(old->convertToFormat(QImage::Format_RGB32));
        delete old;
    }

    assert(temp->height() > 0);
    assert(temp->width() > 0);

    int t_height = temp->height();
    int t_width = temp->width();

    if (m_image)
        delete m_image;
    m_image = new QImage(temp->width(), temp->height(), QImage::Format_RGB32 /* this corresponds to the BGRA struct */);

    // set the new image to black
    memset(m_image->bits(), 0, t_width * t_height * sizeof(BGRA));
    memcpy(m_image->bits(), temp->bits(), temp->numBytes());

//    m_image = temp;

    // Remember the filename so we can revert to it
    m_lastfile = file;

    delete temp;
    return true;
}

bool GLWidget::loadImage(QImage *image, const QString &file)
{
    QImage *temp = new QImage();
    if (!temp->load(file))
    {
        cout << "Failed to load " << file.toStdString() << endl;
        return false;
    }

    // make sure the image is RGB (not monochrome, for example)
    if (temp->format() != QImage::Format_RGB32)
    {
        QImage *old = temp;
        temp = new QImage(old->convertToFormat(QImage::Format_RGB32));
        delete old;
    }

    assert(temp->height() > 0);
    assert(temp->width() > 0);

    int t_height = temp->height();
    int t_width = temp->width();

    if (image)
        delete image;
    image = new QImage(temp->width(), temp->height(), QImage::Format_RGB32 /* this corresponds to the BGRA struct */);

    memset(image->bits(), 0, t_width * t_height * sizeof(BGRA));
    memcpy(image->bits(), temp->bits(), temp->numBytes());

    delete temp;

    assert(image);
    assert(image->height() > 0);
    assert(image->width() > 0);

    cout << "Successfully loaded " << file.toStdString() << endl;
    return true;
}

void GLWidget::paintScene()
{
    //define scene
    float x_start = -2.5;
    float y_start = -1.5;
    float z_start = 0.0;

    float width = 5.0;
    float height = 3.0;
    float depth = 2.0;

    float x_end = x_start + width;
    float y_end = y_start + height;
    float z_end = z_start + depth;

    glBegin( GL_QUADS );
    //define back wall
    glColor3f(0.08, 0.1, 0.05);
    glTexCoord3f(0.0, 0.0, 0.0); glVertex3f(x_start, y_start, z_start);
    glTexCoord3f(1.0, 0.0, 0.0); glVertex3f(x_end, y_start, z_start);
    glTexCoord3f(1.0, 1.0, 0.0); glVertex3f(x_end, y_end, z_start);
    glTexCoord3f(0.0, 1.0, 0.0); glVertex3f(x_start, y_end, z_start);
    glEnd();

    glBegin( GL_QUADS );
    //define top wall
    glColor3f(0.75, 0.5, 0.0);
    glTexCoord3f(0.0, 0.0, 0.0); glVertex3f(x_start, y_end, z_start);
    glTexCoord3f(1.0, 0.0, 0.0); glVertex3f(x_end, y_end, z_start);
    glTexCoord3f(1.0, 1.0, 0.0); glVertex3f(x_end, y_end, z_end);
    glTexCoord3f(0.0, 1.0, 0.0); glVertex3f(x_start, y_end, z_end);
    glEnd();

    glBegin( GL_QUADS );
    //define bottom wall
    glColor3f(0.0, 0.5, 0.5);
    glTexCoord3f(0.0, 0.0, 0.0); glVertex3f(x_end, y_start, z_start);
    glTexCoord3f(1.0, 0.0, 0.0); glVertex3f(x_start, y_start, z_start);
    glTexCoord3f(1.0, 1.0, 0.0); glVertex3f(x_start, y_start, z_end);
    glTexCoord3f(0.0, 1.0, 0.0); glVertex3f(x_end, y_start, z_end);
    glEnd();

    glBegin( GL_QUADS );
    //define right wall
    glColor3f(0.0, 0.0, 0.75);
    glTexCoord3f(0.0, 0.0, 0.0); glVertex3f(x_end, y_start, z_start);
    glTexCoord3f(1.0, 0.0, 0.0); glVertex3f(x_end, y_start, z_end);
    glTexCoord3f(1.0, 1.0, 0.0); glVertex3f(x_end, y_end, z_end);
    glTexCoord3f(0.0, 1.0, 0.0); glVertex3f(x_end, y_end, z_start);
    glEnd();

    glBegin( GL_QUADS );
    //define left wall
    glColor3f(0.0, 0.75, 0.0);
    glTexCoord3f(0.0, 0.0, 0.0); glVertex3f(x_start, y_start, z_end);
    glTexCoord3f(1.0, 0.0, 0.0); glVertex3f(x_start, y_start, z_start);
    glTexCoord3f(1.0, 1.0, 0.0); glVertex3f(x_start, y_end, z_start);
    glTexCoord3f(0.0, 1.0, 0.0); glVertex3f(x_start, y_end, z_end);
    glEnd();
}

void GLWidget::getTimeDigit(QImage *image, int digit)
{
    assert(digit >= 0);
    assert(digit <= 9);
    if (digit == 0)
        image = m_img_zero;
    else if (digit == 1)
        image = m_img_one;
    else if (digit  == 2)
        image = m_img_two;
    else if (digit == 3)
        image = m_img_three;
    else if (digit == 4)
        image == m_img_four;
    else if (digit == 5)
        image = m_img_five;
    else if (digit == 6)
        image = m_img_six;
    else if (digit == 7)
        image = m_img_seven;
    else if (digit == 8)
        image = m_img_eight;
    else if (digit == 9)
        image = m_img_nine;
    else
        image = NULL;
}
