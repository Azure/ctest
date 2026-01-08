// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef TESTNAMEFILTERTESTS_H
#define TESTNAMEFILTERTESTS_H

/* Helper function declarations for filter test tracking (defined in testnamefiltertests.c) */
void FilterTestSuite_ResetExecutionTracking(void);
int FilterTestSuite_WasTest1Executed(void);
int FilterTestSuite_WasTest2Executed(void);
int FilterTestSuite_WasTest3Executed(void);

#endif /* TESTNAMEFILTERTESTS_H */
