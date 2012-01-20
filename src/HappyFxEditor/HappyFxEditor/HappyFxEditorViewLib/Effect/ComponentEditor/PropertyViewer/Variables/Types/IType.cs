﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace HappyFxEditorContextLib.Effect.ComponentEditor.PropertyViewer.Variables.Types
{
    public interface IType : INotifyPropertyChanged
    {
        IType Copy();

        VariableComponentType GetVarType();

        string Name { get; set; }

        void Serialize(BinaryWriter stream);
        void DeSerialize(BinaryReader stream);
    }
}