import Skinny 1.0 as Qsk
import QtQuick 2.5

Item
{
    id: window
    visible: true

    width: QskTextLabel2.width
    height: QskTextLabel2.height

    Qsk.PushButton
    {
        anchors.fill: parent
        text: QskTextLabel2.thing
    }
}
