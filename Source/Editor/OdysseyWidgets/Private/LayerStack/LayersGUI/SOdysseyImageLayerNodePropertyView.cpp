// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

#include "LayerStack/LayersGUI/SOdysseyImageLayerNodePropertyView.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Layout/SBox.h"
#include "OdysseyImageLayer.h"
#include "PropertyEditorModule.h"
#include "Modules/ModuleManager.h"


#define LOCTEXT_NAMESPACE "SOdysseyImageLayerNodePropertyView"

//CONSTRUCTION/DESTRUCTION--------------------------------------

SOdysseyImageLayerNodePropertyView::~SOdysseyImageLayerNodePropertyView()
{
}



void SOdysseyImageLayerNodePropertyView::Construct( const FArguments& InArgs, TSharedRef<IOdysseyBaseLayerNode> iNode )
{
    FOdysseyLayerStack* odysseyLayerStackPtr = iNode->GetLayerStack().GetLayerStackData().Get();
    IOdysseyLayer::eType layerType = iNode->GetLayerDataPtr()->GetType();

    TSharedRef<SWidget> finalWidget = SNullWidget::NullWidget;

    FOdysseyImageLayer* imageLayer = static_cast<FOdysseyImageLayer*> (iNode->GetLayerDataPtr());
    TSharedRef<FOdysseyImageLayerNode> imageNode = StaticCastSharedRef<FOdysseyImageLayerNode>(iNode);
    finalWidget = ConstructPropertyViewForImageLayer( imageLayer, odysseyLayerStackPtr, imageNode );

    ChildSlot
    [
        finalWidget
    ];
}


//PRIVATE API


TSharedRef<SWidget> SOdysseyImageLayerNodePropertyView::ConstructPropertyViewForImageLayer( FOdysseyImageLayer* iImageLayer, FOdysseyLayerStack* iLayerStack, TSharedRef<FOdysseyImageLayerNode> iImageNode )
{
    mBlendingModes = iLayerStack->GetBlendingModesAsText();

    TSharedRef<SWidget>    finalWidget =
        SNew( SVerticalBox )
        + SVerticalBox::Slot()
        [
            SNew(SHorizontalBox )
            + SHorizontalBox::Slot()
            .FillWidth(0.5f)
            [
                SNew( STextBlock )
                .Text(LOCTEXT("Opacity", "Opacity"))
            ]

            +SHorizontalBox::Slot()
            .Padding( FMargin( 20.f, 0.f, 0.f, 0.f) )
            .FillWidth(0.5f)
            .VAlign( VAlign_Center )
            [
                SNew(SSpinBox<int>)
                //.Style( FEditorStyle::Get(), "NoBorder" )
                .Value(this, &SOdysseyImageLayerNodePropertyView::GetLayerOpacityValue, iImageLayer)
                .MinValue(0)
                .MaxValue(100)
                .Delta(1)
                .OnValueChanged(this, &SOdysseyImageLayerNodePropertyView::HandleLayerOpacityValueChanged, iImageLayer, iLayerStack, iImageNode )
                .OnValueCommitted(this, &SOdysseyImageLayerNodePropertyView::SetLayerOpacityValue, iImageLayer, iLayerStack, iImageNode )
             ]
         ]
        + SVerticalBox::Slot()
        .Padding( FMargin( 0.f, 3.f, 0.f, 0.f) )
        [
            SNew(SHorizontalBox )
            + SHorizontalBox::Slot()
            .FillWidth(0.5f)
            [
                SNew( STextBlock )
                .Text(LOCTEXT("Blending Mode", "Blending Mode"))
            ]

             + SHorizontalBox::Slot()
             .Padding( FMargin( 20.f, 0.f, 0.f, 0.f) )
             .FillWidth(0.5f)
             .VAlign( VAlign_Center )
             [
                SAssignNew( mBlendingModeComboBox, SComboBox<TSharedPtr<FText>>)
                .OptionsSource(&mBlendingModes)
                .OnGenerateWidget(this, &SOdysseyImageLayerNodePropertyView::GenerateBlendingComboBoxItem)
                .OnSelectionChanged(this, &SOdysseyImageLayerNodePropertyView::HandleOnBlendingModeChanged, iImageLayer, iLayerStack, iImageNode )
                .Content()
                [
                    //The text in the main button
                    CreateBlendingModeTextWidget( iImageLayer )
                ]
             ]
        ];


    return finalWidget;
}



int SOdysseyImageLayerNodePropertyView::GetLayerOpacityValue( FOdysseyImageLayer* iImageLayer ) const
{
    return iImageLayer->GetOpacity() * 100;
}


void SOdysseyImageLayerNodePropertyView::HandleLayerOpacityValueChanged( int iOpacity, FOdysseyImageLayer* iImageLayer, FOdysseyLayerStack* iLayerStack, TSharedRef<FOdysseyImageLayerNode> iImageNode  )
{
    iImageLayer->SetOpacity( iOpacity / 100.f );
    iImageNode->RefreshOpacityText();
}

void SOdysseyImageLayerNodePropertyView::SetLayerOpacityValue( int iOpacity, ETextCommit::Type iType, FOdysseyImageLayer* iImageLayer, FOdysseyLayerStack* iLayerStack, TSharedRef<FOdysseyImageLayerNode> iImageNode  )
{
    iImageLayer->SetOpacity( iOpacity / 100.f );
    iImageNode->RefreshOpacityText();
    iLayerStack->ComputeResultBlock();
}


//PRIVATE

void SOdysseyImageLayerNodePropertyView::HandleOnBlendingModeChanged(TSharedPtr<FText> iNewSelection, ESelectInfo::Type iSelectInfo, FOdysseyImageLayer* iImageLayer, FOdysseyLayerStack* iLayerStack, TSharedRef<FOdysseyImageLayerNode> iImageNode )
{
    iImageLayer->SetBlendingMode( *(iNewSelection.Get() ) );
    iImageNode->RefreshBlendingModeText();
    iLayerStack->ComputeResultBlock();

    mBlendingModeComboBox->SetContent(
        SNew(   SComboBox<TSharedPtr<FText>> )
                .OptionsSource(&mBlendingModes)
                .OnGenerateWidget(this, &SOdysseyImageLayerNodePropertyView::GenerateBlendingComboBoxItem)
                .OnSelectionChanged(this, &SOdysseyImageLayerNodePropertyView::HandleOnBlendingModeChanged, iImageLayer, iLayerStack, iImageNode )
                .Content()
                [
                    //The text in the main button
                    CreateBlendingModeTextWidget( iImageLayer )
                ]
    );
}



TSharedRef<SWidget> SOdysseyImageLayerNodePropertyView::GenerateBlendingComboBoxItem(TSharedPtr<FText> iItem)
{
      return SNew(STextBlock)
           .Text(*(iItem.Get()));
}


TSharedRef<SWidget> SOdysseyImageLayerNodePropertyView::CreateBlendingModeTextWidget( FOdysseyImageLayer* iImageLayer)
{
      return SNew(STextBlock)
           .Text( iImageLayer->GetBlendingModeAsText() );
}




#undef LOCTEXT_NAMESPACE
