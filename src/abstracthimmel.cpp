
// Copyright (c) 2011-2012, Daniel M�ller <dm@g4t3.de>
// Computer Graphics Systems Group at the Hasso-Plattner-Institute, Germany
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright 
//     notice, this list of conditions and the following disclaimer in the 
//     documentation and/or other materials provided with the distribution.
//   * Neither the name of the Computer Graphics Systems Group at the 
//     Hasso-Plattner-Institute (HPI), Germany nor the names of its 
//     contributors may be used to endorse or promote products derived from 
//     this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.

#include "abstracthimmel.h"

#include "mathmacros.h"
#include "timef.h"

#include <osg/Depth>
#include <osg/NodeVisitor>
#include <osgUtil/CullVisitor>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Matrix>

#include <assert.h>
#include <stdio.h>


#include <osg/BlendFunc>
#include <osg/PositionAttitudeTransform>

osg::PositionAttitudeTransform *pos;


namespace osgHimmel
{

void AbstractHimmel::HimmelNodeCallback::operator()(
    osg::Node *node
,   osg::NodeVisitor *nv)
{
    osg::ref_ptr<AbstractHimmel> himmel = dynamic_cast<AbstractHimmel*>(node);

    if(himmel)
        himmel->update();

    traverse(node, nv); 
}


AbstractHimmel::AbstractHimmel()
:   osg::MatrixTransform()
,   m_timef(NULL)
,   m_autoUpdateTime(false)

,   m_initialized(false)
,   m_dirty(false)

,   m_cameraHint(NULL)
,   m_widthHint(0u)
,   m_heightHint(0u)

,   m_lastElapsed(0.0)

,   m_referenceBoundingRadius(1.f)
{
    setupNode(getOrCreateStateSet());

    setUpdateCallback(new HimmelNodeCallback);


    //osg::Sphere* unitSphere = new osg::Sphere( osg::Vec3(0,0,0), 10.0);
    //osg::ShapeDrawable* unitSphereDrawable = new osg::ShapeDrawable(unitSphere);

    //osg::Sphere* unitSphere1 = new osg::Sphere( osg::Vec3(0,0,0), 10.0);
    //osg::ShapeDrawable* unitSphereDrawable1 = new osg::ShapeDrawable(unitSphere1);


    //pos = new osg::PositionAttitudeTransform();
    //pos->setPosition(osg::Vec3(0.0,0,0));
    //osg::Geode* unitSphereGeode = new osg::Geode();
    //addChild(pos);

    //unitSphereDrawable->setColor(osg::Vec4(0.0, 1.0, 0.0, 1.0));
    //unitSphereDrawable1->setColor(osg::Vec4(1.0, 0.0, 0.0, 0.5));

    //osg::Depth* depth = new osg::Depth(osg::Depth::ALWAYS, 0.0, 1.0);
    //unitSphereDrawable1->getOrCreateStateSet()->setAttributeAndModes(depth, osg::StateAttribute::ON);

    //osg::BlendFunc *blend  = new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE);
    //unitSphereDrawable1->getOrCreateStateSet()->setAttributeAndModes(blend, osg::StateAttribute::ON);

    //pos->addChild(unitSphereGeode);
    //unitSphereGeode->addDrawable(unitSphereDrawable);
    //unitSphereGeode->addDrawable(unitSphereDrawable1);


    setReferenceFrame(osg::Transform::ABSOLUTE_RF);
};


AbstractHimmel::~AbstractHimmel()
{
};


void AbstractHimmel::setReferenceBoundingRadius(const float )
{
    //m_referenceBoundingRadius = _abs(radius);
    //setMatrix(osg::Matrix::scale(m_referenceBoundingRadius, m_referenceBoundingRadius, m_referenceBoundingRadius));
    
    //osg::notify(osg::NOTICE) << m_referenceBoundingRadius << std::endl;
}

//int ffff = 0;
void AbstractHimmel::traverse(osg::NodeVisitor &nv)
{
    osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(&nv);
    if(!cv)
        return;



    //osg::Vec3 eye, center, up;
    //cv->getCurrentCamera()->getViewMatrixAsLookAt(eye, center, up);

    //const float n(cv->getCalculatedNearPlane());
    //const float f(cv->getCalculatedFarPlane());

    //center.normalize();

    //   if(((ffff) % 100) == 0)
    //   {
    //    osg::notify (osg::NOTICE) << "traverse" << std::endl;
    //   osg::notify (osg::NOTICE) << "center \t" << center.x() << " \t" << center.y() << " \t" << center.z() << std::endl;
    //   osg::notify (osg::NOTICE) << "eye    \t" << eye.x() << " \t" << eye.y() << " \t" << eye.z() << std::endl;
    //   osg::notify (osg::NOTICE) << "up     \t" << up.x() << " \t" << up.y() << " \t" << up.z() << std::endl;
    //   osg::notify (osg::NOTICE) << "near/f \t" << n << " \t" << f << std::endl;
    //   }


    const osg::CullSettings::ComputeNearFarMode cnfm(cv->getCurrentCamera()->getComputeNearFarMode());
    cv->getCurrentCamera()->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);

    osg::MatrixTransform::traverse(nv);

    cv->getCurrentCamera()->setComputeNearFarMode(cnfm);
}



float x = 0.0;
bool AbstractHimmel::computeLocalToWorldMatrix(osg::Matrix& matrix, osg::NodeVisitor* nv) const
{
    osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
    if(!cv)
        return false;

    const osg::Vec3 t(cv->getEyePoint());
    matrix.preMultTranslate(t);


    //osg::Vec3 eye, center, up;
    //cv->getCurrentCamera()->getViewMatrixAsLookAt(eye, center, up);

    //const float n(cv->getCalculatedNearPlane());
    //const float f(cv->getCalculatedFarPlane());

    //center.normalize();
//        osg::Vec3 i = eye - center * (f + n) * 0.5;
//
//  //      //const osg::Vec3 t( ->getEyePoint());
//  //      //matrix.preMultTranslate(t);
//  //      
//  //      osg::Matrixd m = osg::Matrixd::inverse(
//  //      cv->getCurrentCamera()->getInverseViewMatrix());
//
//  //      m.getLookAt(eye, center, up);
//
//  //          //m_recorder->record(rc::PathPoint(eye._v, center._v, up._v, fov));
//
//  ////      osg::Matrix m = matrix;
//  //      //matrix.inverse(m);
//  //      osg::Vec3 i = center - (eye - center) * 100.0;
//
//
       //if(((ffff++) % 100) == 0)
       //{
       // osg::notify (osg::NOTICE) << "computeLocalToWorldMatrix" << std::endl;
       //osg::notify (osg::NOTICE) << "center \t" << center.x() << " \t" << center.y() << " \t" << center.z() << std::endl;
       //osg::notify (osg::NOTICE) << "eye    \t" << eye.x() << " \t" << eye.y() << " \t" << eye.z() << std::endl;
       //osg::notify (osg::NOTICE) << "up     \t" << up.x() << " \t" << up.y() << " \t" << up.z() << std::endl;
       //osg::notify (osg::NOTICE) << "near/f \t" << n << " \t" << f << std::endl;
       //}
////        m.getLookAt(eye, center, up, 10.0);
//        //
//        pos->setPosition(i);

    return true;
//    return false;
}


bool AbstractHimmel::computeWorldToLocalMatrix(osg::Matrix& matrix, osg::NodeVisitor* nv) const
{
    osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
    if(!cv)
        return false;

    const osg::Vec3 t(cv->getEyePoint());
    matrix.preMultTranslate(-t);

    return true;
}


void AbstractHimmel::setupNode(osg::StateSet* stateSet)
{
    setCullingActive(false);

    // Only draw at back plane.
    osg::Depth* depth = new osg::Depth(osg::Depth::LEQUAL, 1.0, 1.0);
    stateSet->setAttributeAndModes(depth, osg::StateAttribute::ON);
}


void AbstractHimmel::initialize()
{
    if(m_initialized)
        return;

    postInitialize();

    m_initialized = true;
}


void AbstractHimmel::update()
{
    if(!m_initialized)
        initialize();

    if(m_autoUpdateTime && m_timef)
    {
        m_timef->update();
        dirty();
    }


    if(!m_timef)
        return;

    const t_longf elapsed(m_timef->getNonModf());
    if(elapsed != m_lastElapsed)
    {
        dirty();
        m_lastElapsed = elapsed;
    }
}


void AbstractHimmel::assignTime(
    TimeF *timef
,   const bool autoUpdate)
{
    m_timef = timef;
    m_autoUpdateTime = autoUpdate;
}


void AbstractHimmel::setAutoUpdateTime(const bool autoUpdate)
{
    m_autoUpdateTime = autoUpdate;
}


const float AbstractHimmel::timef() const
{
    if(m_timef)
        return m_timef->getf();

    return 0.f;
}


void AbstractHimmel::dirty(const bool enable)
{
    m_dirty = enable;
}


const bool AbstractHimmel::isDirty() const
{
    return m_dirty;
}




void AbstractHimmel::setCameraHint(osg::Camera *camera)
{
    dirty();

    m_cameraHint = camera;
}

const float AbstractHimmel::getCameraFovHint() const
{
    assert(m_cameraHint);
    
    double fov, dummy;
    if(m_cameraHint->getProjectionMatrixAsPerspective(fov, dummy, dummy, dummy))
        return fov;

    return 0;
}


void AbstractHimmel::setViewSizeHint(unsigned int width, unsigned int height)
{
    dirty();

    m_widthHint = width;
    m_heightHint = height;
}

const unsigned int AbstractHimmel::getViewSizeWidthHint() const
{
    return m_widthHint;
}

const unsigned int AbstractHimmel::getViewSizeHeightHint() const
{
    return m_heightHint;
}

} // namespace osgHimmel