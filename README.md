qmlseen
=======

Mini tool to generate prerendered images from QML file.

Usage
-----

Add `property string fileName` to items to be grabbed, and run:

    $ qmlseen source.qml

See [contrib/shadows.qml](contrib/shadows.qml) for example.

The root item must not be 0x0 size. Using `Grid` should be good for
debugging.
