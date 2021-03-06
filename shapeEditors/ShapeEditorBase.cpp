#include "ShapeEditorBase.h"
#include "scaleaction.h"
#include "polylineeditor.h"
#include "texteditor.h"
#include "BaseShapeEditor.h"
#include "MultiShapesEditor.h"
#include "polygoneditor.h"

ShapeEditorBase::ShapeEditorBase()
{
	m_hasPicked = 0;
}


ShapeEditorBase::~ShapeEditorBase()
{
}

bool ShapeEditorBase::contextMenu(QContextMenuEvent * event)
{
	return false;
}

QList<shared_ptr<GeneralShape>>& ShapeEditorBase::shapes()
{
	return m_shapes;
}

shared_ptr<GeneralShape>& ShapeEditorBase::shape()
{
	return m_shape;
}

AbstractAction*& ShapeEditorBase::action()
{
	return m_action;
}

void ShapeEditorBase::setCallBack(ICallBack * callBack)
{
	m_callBack = callBack;
}

double & ShapeEditorBase::zoomRatio()
{
	return m_zoomRatio;
}

void ShapeEditorBase::drawControlPoints(QPainter & painter, qreal zoomRatio)
{
}
void ShapeEditorBase::publishEditFinished()
{
	m_callBack->call("editFinished");
}

bool ShapeEditorBase::keyDown(QKeyEvent * event)
{
	return false;
}

bool ShapeEditorBase::keyUp(QKeyEvent * event)
{
	return false;
}

QList<QAction*> ShapeEditorBase::QActions(QPointF p, QContextMenuEvent * event)
{
	return QList<QAction*>();
}

void ShapeEditorBase::startScale(QPointF p)
{
	m_scalePointId = scaleHandlePointId(p, m_shape, m_zoomRatio);
	currentMouseHanded = ScalePoint;
	//startPoint=realPoint;
	endPoint = startPoint = m_shape->scaleHandlePoints()[m_scalePointId];
	m_scaleCenter = m_shape->scaleHandlePoints()[(m_scalePointId + 2) % 4];
}

void ShapeEditorBase::doScale(QPointF realPoint)
{
	m_shape->scale(m_scalePointId, realPoint);
	endPoint = realPoint;
}

void ShapeEditorBase::finishScale(QPointF realPoint)
{
	currentMouseHanded = None;

	ScaleAction* action = new ScaleAction();
	action->shapes.append(m_shape);
	action->setScaleAction(m_scalePointId, startPoint, realPoint);

	AbstractAction* tmp = static_cast<AbstractAction*>(action);
	tmp->m_bHasDone = true;
	m_action = tmp;
	publishEditFinished();
}

void ShapeEditorBase::resetHasPicked()
{
	m_hasPicked = 0;
}

SHAPEEDITORS_EXPORT void updateIShapeEditor(const QList<shared_ptr<GeneralShape>> pickedShapes,
	shared_ptr<IShapeEditor>& curIShapeEditor)
{
	QString shapename = pickedShapes[0]->name();
	if (curIShapeEditor != nullptr &&
		((pickedShapes.size() == 1 && curIShapeEditor->shapes().size() == 1 && curIShapeEditor->shape()->name() == shapename) ||
		(curIShapeEditor->shapes().size() > 1 && dynamic_pointer_cast<MultiShapesEditor>(curIShapeEditor))
			)) {
		if (pickedShapes.size() == 1 && curIShapeEditor->shapes()[0] != pickedShapes[0]) {
			curIShapeEditor->resetHasPicked();
			curIShapeEditor->shapes() = pickedShapes;
			curIShapeEditor->shape() = pickedShapes.at(0);
		}
		return;
	}
	if (pickedShapes.size() > 1) {
		curIShapeEditor = shared_ptr<MultiShapesEditor>(new MultiShapesEditor);
	}
	else if (shapename == "Polyline") {
		curIShapeEditor = shared_ptr<PolylineEditor>(new PolylineEditor);
	}
	else if (shapename == "Polygon") {
		curIShapeEditor = shared_ptr<PolygonEditor>(new PolygonEditor);
	}
	else if (shapename == "Text") {
		curIShapeEditor = shared_ptr<TextEditor>(new TextEditor);
	}
	else {
		curIShapeEditor = shared_ptr<BaseShapeEditor>(new BaseShapeEditor);
	}
	curIShapeEditor->shapes() = pickedShapes;
	curIShapeEditor->shape() = pickedShapes.at(0);
}
