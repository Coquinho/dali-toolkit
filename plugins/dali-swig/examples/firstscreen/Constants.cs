﻿using System;

namespace FirstScreen
{
    public class Constants
    {
        public const int TotalPostersContainers = 2;                  // Number of Poster ScrollContainers to be added on Top Container

        public const float TopContainerHeightFactor = 0.38f;          // Height Factor of stage height used for Top Container total height
        public const float TopContainerPositionFactor = 0.50f;        // Position Factor of stage height used for Top Container position
        public const float TopClipLayerHeightFactor = 0.34f;          // Height Factor of stage height used for Top Clip layer height
        public const float TopClipLayerExpandHeightFactor = 0.36f;    // Expanded Height Factor of stage height used for Top Clip layer height (used when Bottom container is hidden)
        public const float PostersContainerHeightFactor = 0.32f;      // Height Factor of stage height used for Poster ScrollContainers
        public const float PostersContainerPadding = 2.0f;            // Padding size used between items / images in Poster ScrollContainer
        public const float PostersContainerMargin = 60.0f;            // Extra margin Padding size used between items / images in Poster ScrollContainer when item / image is focused
        public const float PostersContainerOffsetYFactor = 0.17f;     // Position Factor of Poster item height used for Poster items / images position

        public const float BottomContainerHeightFactor = 0.16f;       // Height Factor of stage height used for Bottom Container total height
        public const float BottomContainerHidePositionFactor = 0.88f; // Position Factor of stage height used for Bottom Container position when bottom container is hidden (not focused)
        public const float BottomContainerShowPositionFactor = 0.84f; // Position Factor of stage height used for Bottom Container position when bottom container is not hidden (focused)
        public const float MenuContainerHeightFactor = 0.16f;         // Height Factor of stage height used for Bottom ScrollContainers
        public const float BottomClipLayerHeightFactor = 0.84f;       // Height Factor of stage height used for Bottom Clip layer height
        public const float MenuContainerPadding = 10.0f;              // Padding size used between items / images in Menu ScrollContainer
        public const float MenuContainerMargin = 25.0f;               // Extra margin Padding size used between items / images in Menu ScrollContainer when item / image is focused
        public const float MenuContainerOffsetYFactor = 0.35f;        // Position Factor of Poster item height used for Menu items / images position

        public const float MenuItemWidthFactor = 0.125f;              // Width Factor (1/8) of stage Width used for Menu items / images Width
        public const float MenuItemHeightFactor = 0.10f;              // Height Factor of stage height used for Menu items / images Height
        public const float MenuItemsCount = 14;                       // Number of Menu items / images used in a Menu ScrollContainer

        public const float Poster0ItemWidthFactor = 0.25f;            // Width Factor (1/4) of stage Width used for Poster items / images Width in a Poster ScrollContainer 0
        public const float Poster1ItemWidthFactor = 0.24f;            // Width Factor of stage Width used for Poster items / images Width in a Poster ScrollContainer 1
        public const float PostersItemHeightFactor = 0.24f;           // Height Factor of stage height used for Poster items / images Height
        public const float PostersItemsCount = 24;                    // Number of Menu items / images used in a Poster ScrollContainer

        public const float LauncherWidth = 280.0f;                    // Width of Launcher. Launcher is used to display three images on left of Menu ScrollContainer
        public const float LauncherLeftMargin = 40.0f;                // Extra Spaces to the left of first Launcher item / image
        public const float LauncherSeparatorWidth = 20.0f;            // Extra area / space to the left of Menu ScrollContainer used for a speration shadow image
        public const float LauncherItemsCount = 3.0f;                 // Total number of Launcher items / images
        public const float LauncherIconWidth = (LauncherWidth - LauncherLeftMargin - LauncherSeparatorWidth) / LauncherItemsCount; // Width of each Launcher item / image
    }
}

