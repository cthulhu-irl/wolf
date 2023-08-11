import QtQuick
import QtQuick.Controls
import wolf_example_gametime

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    q_custom_render_item {
        id: render_item
        anchors.fill: parent
    }

    Label {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 20
        background: Rectangle { color: "black" }
        color: "white"
        font.bold: true
        text: "FPS: %1 | Total Elapsed Time: %2 seconds"
                .arg(render_item.fps)
                .arg(render_item.total_time)
    }
}
