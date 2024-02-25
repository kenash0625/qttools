import Skinny 1.0 as Qsk
import QtQuick 2.5

Item
{
    id: window
    visible: true

    width: QskProgressBar2.width
    height: QskProgressBar2.height

    Qsk.ProgressBar
    {
        anchors.fill: parent
        minimum: QskProgressBar2.minvalue
        maximum: QskProgressBar2.maxvalue
        orientation:QskProgressBar2.orientation
    }

}
