import QtGraphicalEffects 1.12
import QtQuick 2.12

Grid {
    spacing: 10

    Repeater {
        model: 5

        // content rect
        Item {
            property string fileName: "shadow-" + shadow.width + "x" + shadow.height + ".png"

            width: shadow.width + 10
            height: shadow.height + 10

            DropShadow {
                id: shadow
                anchors.centerIn: parent
                width: 120
                height: 25 * (model.index + 1)
                source: Rectangle {
                    width: shadow.width
                    height: shadow.height
                }
                radius: 3
                samples: 7
            }

            Text {
                anchors.centerIn: parent
                text: fileName
            }
        }
    }
}
