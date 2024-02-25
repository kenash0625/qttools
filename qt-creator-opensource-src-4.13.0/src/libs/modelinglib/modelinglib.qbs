import qbs 1.0

QtcLibrary {
    name: "Modeling"

    cpp.defines: base.concat("MODELING_LIBRARY")
    cpp.includePaths: base.concat([
        ".",
        "./qtserialization/inc",
    ])

    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "Qt.svg"; required: false }
    Depends { name: "Utils" }

    Properties {
        condition: !Qt.svg.present
        cpp.defines: base.concat("QT_NO_SVG")
    }

    Group {
        name: "Qmt"
        prefix: "./qmt/"
        files: [
            "config/configcontroller.cpp",
            "config/configcontroller.h",
            "config/sourcepos.cpp",
            "config/sourcepos.h",
            "config/stereotypedefinitionparser.cpp",
            "config/stereotypedefinitionparser.h",
            "config/stringtextsource.cpp",
            "config/stringtextsource.h",
            "config/textscanner.cpp",
            "config/textscanner.h",
            "config/textsource.h",
            "config/token.cpp",
            "config/token.h",
            "controller/container.h",
            "controller/namecontroller.cpp",
            "controller/namecontroller.h",
            "controller/references.h",
            "controller/selection.cpp",
            "controller/selection.h",
            "controller/undocommand.cpp",
            "controller/undocommand.h",
            "controller/undocontroller.cpp",
            "controller/undocontroller.h",
            "diagram/dannotation.cpp",
            "diagram/dannotation.h",
            "diagram/dassociation.cpp",
            "diagram/dassociation.h",
            "diagram/dboundary.cpp",
            "diagram/dboundary.h",
            "diagram/dclass.cpp",
            "diagram/dclass.h",
            "diagram/dcomponent.cpp",
            "diagram/dcomponent.h",
            "diagram/dconnection.cpp",
            "diagram/dconnection.h",
            "diagram/dconstvisitor.h",
            "diagram/ddependency.cpp",
            "diagram/ddependency.h",
            "diagram/ddiagram.cpp",
            "diagram/ddiagram.h",
            "diagram/delement.cpp",
            "diagram/delement.h",
            "diagram/dinheritance.cpp",
            "diagram/dinheritance.h",
            "diagram/ditem.cpp",
            "diagram/ditem.h",
            "diagram/dobject.cpp",
            "diagram/dobject.h",
            "diagram/dpackage.cpp",
            "diagram/dpackage.h",
            "diagram/drelation.cpp",
            "diagram/drelation.h",
            "diagram/dswimlane.cpp",
            "diagram/dswimlane.h",
            "diagram/dvisitor.h",
            "diagram_controller/dclonevisitor.cpp",
            "diagram_controller/dclonevisitor.h",
            "diagram_controller/dcontainer.h",
            "diagram_controller/dfactory.cpp",
            "diagram_controller/dfactory.h",
            "diagram_controller/dflatassignmentvisitor.cpp",
            "diagram_controller/dflatassignmentvisitor.h",
            "diagram_controller/diagramcontroller.cpp",
            "diagram_controller/diagramcontroller.h",
            "diagram_controller/dreferences.h",
            "diagram_controller/dselection.h",
            "diagram_controller/dupdatevisitor.cpp",
            "diagram_controller/dupdatevisitor.h",
            "diagram_controller/dvoidvisitor.cpp",
            "diagram_controller/dvoidvisitor.h",
            "diagram_scene/capabilities/alignable.h",
            "diagram_scene/capabilities/editable.h",
            "diagram_scene/capabilities/intersectionable.h",
            "diagram_scene/capabilities/latchable.h",
            "diagram_scene/capabilities/moveable.h",
            "diagram_scene/capabilities/relationable.h",
            "diagram_scene/capabilities/resizable.h",
            "diagram_scene/capabilities/selectable.h",
            "diagram_scene/capabilities/windable.h",
            "diagram_scene/diagramgraphicsscene.cpp",
            "diagram_scene/diagramgraphicsscene.h",
            "diagram_scene/diagramsceneconstants.h",
            "diagram_scene/diagramscenemodel.cpp",
            "diagram_scene/diagramscenemodel.h",
            "diagram_scene/diagramscenemodelitemvisitors.cpp",
            "diagram_scene/diagramscenemodelitemvisitors.h",
            "diagram_scene/items/annotationitem.cpp",
            "diagram_scene/items/annotationitem.h",
            "diagram_scene/items/associationitem.cpp",
            "diagram_scene/items/associationitem.h",
            "diagram_scene/items/boundaryitem.cpp",
            "diagram_scene/items/boundaryitem.h",
            "diagram_scene/items/classitem.cpp",
            "diagram_scene/items/classitem.h",
            "diagram_scene/items/componentitem.cpp",
            "diagram_scene/items/componentitem.h",
            "diagram_scene/items/connectionitem.cpp",
            "diagram_scene/items/connectionitem.h",
            "diagram_scene/items/diagramitem.cpp",
            "diagram_scene/items/diagramitem.h",
            "diagram_scene/items/itemitem.cpp",
            "diagram_scene/items/itemitem.h",
            "diagram_scene/items/objectitem.cpp",
            "diagram_scene/items/objectitem.h",
            "diagram_scene/items/packageitem.cpp",
            "diagram_scene/items/packageitem.h",
            "diagram_scene/items/relationitem.cpp",
            "diagram_scene/items/relationitem.h",
            "diagram_scene/items/stereotypedisplayvisitor.cpp",
            "diagram_scene/items/stereotypedisplayvisitor.h",
            "diagram_scene/items/swimlaneitem.cpp",
            "diagram_scene/items/swimlaneitem.h",
            "diagram_scene/latchcontroller.cpp",
            "diagram_scene/latchcontroller.h",
            "diagram_scene/parts/alignbuttonsitem.cpp",
            "diagram_scene/parts/alignbuttonsitem.h",
            "diagram_scene/parts/alignlineitem.cpp",
            "diagram_scene/parts/alignlineitem.h",
            "diagram_scene/parts/arrowitem.cpp",
            "diagram_scene/parts/arrowitem.h",
            "diagram_scene/parts/contextlabelitem.cpp",
            "diagram_scene/parts/contextlabelitem.h",
            "diagram_scene/parts/customiconitem.cpp",
            "diagram_scene/parts/customiconitem.h",
            "diagram_scene/parts/editabletextitem.cpp",
            "diagram_scene/parts/editabletextitem.h",
            "diagram_scene/parts/pathselectionitem.cpp",
            "diagram_scene/parts/pathselectionitem.h",
            "diagram_scene/parts/rectangularselectionitem.cpp",
            "diagram_scene/parts/rectangularselectionitem.h",
            "diagram_scene/parts/relationstarter.cpp",
            "diagram_scene/parts/relationstarter.h",
            "diagram_scene/parts/stereotypesitem.cpp",
            "diagram_scene/parts/stereotypesitem.h",
            "diagram_scene/parts/templateparameterbox.cpp",
            "diagram_scene/parts/templateparameterbox.h",
            "diagram_ui/diagram_mime_types.h",
            "diagram_ui/diagramsmanager.cpp",
            "diagram_ui/diagramsmanager.h",
            "diagram_ui/diagramsviewinterface.h",
            "diagram_ui/sceneinspector.cpp",
            "diagram_ui/sceneinspector.h",
            "diagram_widgets_ui/diagramsview.cpp",
            "diagram_widgets_ui/diagramsview.h",
            "diagram_widgets_ui/diagramview.cpp",
            "diagram_widgets_ui/diagramview.h",
            "diagram_widgets_ui/stackeddiagramsview.cpp",
            "diagram_widgets_ui/stackeddiagramsview.h",
            "document_controller/documentcontroller.cpp",
            "document_controller/documentcontroller.h",
            "infrastructure/contextmenuaction.cpp",
            "infrastructure/contextmenuaction.h",
            "infrastructure/exceptions.cpp",
            "infrastructure/exceptions.h",
            "infrastructure/geometryutilities.cpp",
            "infrastructure/geometryutilities.h",
            "infrastructure/handle.h",
            "infrastructure/handles.h",
            "infrastructure/ioexceptions.cpp",
            "infrastructure/ioexceptions.h",
            "infrastructure/qcompressedfile.cpp",
            "infrastructure/qcompressedfile.h",
            "infrastructure/qmtassert.h",
            "infrastructure/uid.h",
            "model/massociation.cpp",
            "model/massociation.h",
            "model/mcanvasdiagram.cpp",
            "model/mcanvasdiagram.h",
            "model/mclass.cpp",
            "model/mclass.h",
            "model/mclassmember.cpp",
            "model/mclassmember.h",
            "model/mcomponent.cpp",
            "model/mcomponent.h",
            "model/mconnection.cpp",
            "model/mconnection.h",
            "model/mconstvisitor.h",
            "model/mdependency.cpp",
            "model/mdependency.h",
            "model/mdiagram.cpp",
            "model/mdiagram.h",
            "model/melement.cpp",
            "model/melement.h",
            "model/minheritance.cpp",
            "model/minheritance.h",
            "model/mitem.cpp",
            "model/mitem.h",
            "model/mobject.cpp",
            "model/mobject.h",
            "model/mpackage.cpp",
            "model/mpackage.h",
            "model/mrelation.cpp",
            "model/mrelation.h",
            "model/msourceexpansion.cpp",
            "model/msourceexpansion.h",
            "model/mvisitor.h",
            "model_controller/mchildrenvisitor.cpp",
            "model_controller/mchildrenvisitor.h",
            "model_controller/mclonevisitor.cpp",
            "model_controller/mclonevisitor.h",
            "model_controller/mcontainer.h",
            "model_controller/mflatassignmentvisitor.cpp",
            "model_controller/mflatassignmentvisitor.h",
            "model_controller/modelcontroller.cpp",
            "model_controller/modelcontroller.h",
            "model_controller/mreferences.h",
            "model_controller/mselection.h",
            "model_controller/mvoidvisitor.cpp",
            "model_controller/mvoidvisitor.h",
            "model_ui/modeltreeviewinterface.h",
            "model_ui/sortedtreemodel.cpp",
            "model_ui/sortedtreemodel.h",
            "model_ui/stereotypescontroller.cpp",
            "model_ui/stereotypescontroller.h",
            "model_ui/treemodel.cpp",
            "model_ui/treemodel.h",
            "model_ui/treemodelmanager.cpp",
            "model_ui/treemodelmanager.h",
            "model_widgets_ui/classmembersedit.cpp",
            "model_widgets_ui/classmembersedit.h",
            "model_widgets_ui/modeltreeview.cpp",
            "model_widgets_ui/modeltreeview.h",
            "model_widgets_ui/palettebox.cpp",
            "model_widgets_ui/palettebox.h",
            "model_widgets_ui/propertiesview.cpp",
            "model_widgets_ui/propertiesview.h",
            "model_widgets_ui/propertiesviewmview.cpp",
            "model_widgets_ui/propertiesviewmview.h",
            "project/project.cpp",
            "project/project.h",
            "project_controller/projectcontroller.cpp",
            "project_controller/projectcontroller.h",
            "serializer/diagramserializer.cpp",
            "serializer/diagramserializer.h",
            "serializer/infrastructureserializer.cpp",
            "serializer/infrastructureserializer.h",
            "serializer/modelserializer.cpp",
            "serializer/modelserializer.h",
            "serializer/projectserializer.cpp",
            "serializer/projectserializer.h",
            "stereotype/customrelation.cpp",
            "stereotype/customrelation.h",
            "stereotype/iconshape.cpp",
            "stereotype/iconshape.h",
            "stereotype/shape.h",
            "stereotype/shapepaintvisitor.cpp",
            "stereotype/shapepaintvisitor.h",
            "stereotype/shapes.cpp",
            "stereotype/shapes.h",
            "stereotype/shapevalue.cpp",
            "stereotype/shapevalue.h",
            "stereotype/shapevisitor.h",
            "stereotype/stereotypecontroller.cpp",
            "stereotype/stereotypecontroller.h",
            "stereotype/stereotypeicon.cpp",
            "stereotype/stereotypeicon.h",
            "stereotype/toolbar.cpp",
            "stereotype/toolbar.h",
            "style/defaultstyle.cpp",
            "style/defaultstyle.h",
            "style/defaultstyleengine.cpp",
            "style/defaultstyleengine.h",
            "style/objectvisuals.cpp",
            "style/objectvisuals.h",
            "style/relationstarterstyle.cpp",
            "style/relationstarterstyle.h",
            "style/style.cpp",
            "style/style.h",
            "style/stylecontroller.cpp",
            "style/stylecontroller.h",
            "style/styledobject.cpp",
            "style/styledobject.h",
            "style/styledrelation.cpp",
            "style/styledrelation.h",
            "style/styleengine.h",
            "tasks/alignonrastervisitor.cpp",
            "tasks/alignonrastervisitor.h",
            "tasks/diagramscenecontroller.cpp",
            "tasks/diagramscenecontroller.h",
            "tasks/finddiagramvisitor.cpp",
            "tasks/finddiagramvisitor.h",
            "tasks/findrootdiagramvisitor.cpp",
            "tasks/findrootdiagramvisitor.h",
            "tasks/ielementtasks.h",
            "tasks/isceneinspector.h",
            "tasks/voidelementtasks.cpp",
            "tasks/voidelementtasks.h",
        ]
    }

    Group {
        name: "QStringParser"
        prefix: "./qstringparser/"
        files: [
            "qstringparser.cpp",
            "qstringparser.h",
        ]
    }

    Group {
        name: "QtSerialization"
        prefix: "./qtserialization/"
        files: [
            "inc/qark/access.h",
            "inc/qark/archivebasics.h",
            "inc/qark/attribute.h",
            "inc/qark/baseclass.h",
            "inc/qark/flag.h",
            "inc/qark/friend_access.h",
            "inc/qark/impl/loadingrefmap.h",
            "inc/qark/impl/objectid.h",
            "inc/qark/impl/savingrefmap.h",
            "inc/qark/parameters.h",
            "inc/qark/qxmlinarchive.h",
            "inc/qark/qxmloutarchive.h",
            "inc/qark/reference.h",
            "inc/qark/serialize.h",
            "inc/qark/serialize_basic.h",
            "inc/qark/serialize_container.h",
            "inc/qark/serialize_enum.h",
            "inc/qark/serialize_pointer.h",
            "inc/qark/tag.h",
            "inc/qark/typeregistry.h",
            "src/flag.cpp",
            "src/savingrefmap.cpp",
        ]
    }

    Group {
        name: "Images"
        prefix: "./qmt/resources/"
        files: [
            "resources.qrc",
            "25x25/align-bottom.png",
            "25x25/align-horizontal.png",
            "25x25/align-left.png",
            "25x25/align-right.png",
            "25x25/align-top.png",
            "25x25/align-vertical.png",
            "25x25/even-horizontal.png",
            "25x25/even-vertical.png",
            "25x25/same-height.png",
            "25x25/same-width.png",
            "48x48/annotation.png",
            "48x48/association.png",
            "48x48/boundary.png",
            "48x48/canvas-diagram.png",
            "48x48/class.png",
            "48x48/component.png",
            "48x48/dependency.png",
            "48x48/generic.png",
            "48x48/inheritance.png",
            "48x48/item.png",
            "48x48/package.png",
        ]
    }

    Export {
        cpp.includePaths: [
            ".",
            "./qtserialization/inc"
        ]
    }
}
