#include"JsValue.h"
#include"JsSys.h"
#include"JsDebug.h"
#include"JsAst.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>



/************************************************/
static void JsGcMarkAstNode(void* mp,int ms){
	struct JsAstNode* p = (struct JsAstNode*)mp;
	JsGcMark(p->location);
};
struct JsAstNode* JsCreateAstNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstNode),&JsGcMarkAstNode,NULL);
	node->astClass = type;
	node->location = l;
	return node;
}

/************************************************/
static void JsGcMarkAstLiteralNode(void* mp,int ms){
	struct JsAstLiteralNode* p = (struct JsAstLiteralNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->value);
}
struct JsAstLiteralNode* JsCreateAstLiteralNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstLiteralNode),&JsGcMarkAstLiteralNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstLiteralNode*)node;
}
/************************************************/
static void JsGcMarkAstStringLiteralNode(void* mp,int ms){
	struct JsAstStringLiteralNode* p = (struct JsAstStringLiteralNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->string);
}
struct JsAstStringLiteralNode* JsCreateAstStringLiteralNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstStringLiteralNode),&JsGcMarkAstStringLiteralNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstStringLiteralNode*)node;
}
/************************************************/
static void JsGcMarkAstPrimaryExpressionIdentNode(void* mp,int ms){
	struct JsAstPrimaryExpressionIdentNode* p = (struct JsAstPrimaryExpressionIdentNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->string);
}
struct JsAstPrimaryExpressionIdentNode* JsCreateAstPrimaryExpressionIdentNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstPrimaryExpressionIdentNode),&JsGcMarkAstPrimaryExpressionIdentNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstPrimaryExpressionIdentNode*)node;

}

/************************************************/

static void JsGcMarkAstArrayLiteralNode(void* mp,int ms){
	struct JsAstArrayLiteralNode* p = (struct JsAstArrayLiteralNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->first);
}
struct JsAstArrayLiteralNode* JsCreateAstArrayLiteralNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstArrayLiteralNode),&JsGcMarkAstArrayLiteralNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstArrayLiteralNode*)node;
}

static void JsGcMarkAstArrayLiteralElement(void* mp,int ms){
	struct JsAstArrayLiteralElement* p = (struct JsAstArrayLiteralElement*)mp;
	JsGcMark(p->expr);
	JsGcMark(p->next);
}
struct JsAstArrayLiteralElement* JsCreateAstArrayLiteralElement(){
	struct JsAstArrayLiteralElement* ele = (struct JsAstArrayLiteralElement*)JsGcMalloc(sizeof(struct JsAstArrayLiteralElement),&JsGcMarkAstArrayLiteralElement,NULL);
	return ele;
}
/************************************************/
static void JsGcMarkAstObjectLiteralNode(void* mp,int ms){
	struct JsAstObjectLiteralNode* p = (struct JsAstObjectLiteralNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->first);
}
struct JsAstObjectLiteralNode* JsCreateAstObjectLiteralNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstObjectLiteralNode),&JsGcMarkAstObjectLiteralNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstObjectLiteralNode*)node;
}


static void JsGcMarkAstObjectLiteralPair(void* mp,int ms){
	struct JsAstObjectLiteralPair* p = (struct JsAstObjectLiteralPair*)mp;
	JsGcMark(p->value);
	JsGcMark(p->next);
	JsGcMark(p->name);
}
struct JsAstObjectLiteralPair* JsCreateAstObjectLiteralPair(){
	struct JsAstObjectLiteralPair* ele =(struct JsAstObjectLiteralPair*) JsGcMalloc(sizeof(struct JsAstObjectLiteralPair),&JsGcMarkAstObjectLiteralPair,NULL);
	return ele;
}
/************************************************/
static void JsGcMarAstArgumentsNode(void* mp,int ms){
	struct JsAstArgumentsNode* p = (struct JsAstArgumentsNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->first);
}
struct JsAstArgumentsNode* JsCreateAstArgumentsNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstArgumentsNode),&JsGcMarAstArgumentsNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstArgumentsNode*)node;
}

static void JsGcMarkAstArgumentsArg(void* mp,int ms){
	struct JsAstArgumentsArg* p = (struct JsAstArgumentsArg*)mp;
	JsGcMark(p->expr);
	JsGcMark(p->next);
}
struct JsAstArgumentsArg* JsCreateAstArgumentsArg(){
	struct JsAstArgumentsArg* ele =(struct JsAstArgumentsArg*) JsGcMalloc(sizeof(struct JsAstArgumentsArg),&JsGcMarkAstArgumentsArg,NULL);
	return ele;
}

/************************************************/
static void JsGcMarkAstMemberExpressionNewNode(void* mp,int ms){
	struct JsAstMemberExpressionNewNode* p = (struct JsAstMemberExpressionNewNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->mexp);
	JsGcMark(p->args);
}
struct JsAstMemberExpressionNewNode* JsCreateAstMemberExpressionNewNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstMemberExpressionNewNode),&JsGcMarkAstMemberExpressionNewNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstMemberExpressionNewNode*)node;
}

/************************************************/
static void JsGcMarkAstMemberExpressionDotNode(void* mp,int ms){
	struct JsAstMemberExpressionDotNode* p = (struct JsAstMemberExpressionDotNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->mexp);
	JsGcMark(p->name);
}
struct JsAstMemberExpressionDotNode* JsCreateAstMemberExpressionDotNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstMemberExpressionDotNode),&JsGcMarkAstMemberExpressionDotNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstMemberExpressionDotNode*)node;

}

/************************************************/

static void JsGcMarkMemberExpressionBracketNode(void* mp,int ms){
	struct JsAstMemberExpressionBracketNode* p = (struct JsAstMemberExpressionBracketNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->mexp);
	JsGcMark(p->name);
}
struct JsAstMemberExpressionBracketNode* JsCreateAstMemberExpressionBracketNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstMemberExpressionBracketNode),&JsGcMarkMemberExpressionBracketNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstMemberExpressionBracketNode*)node;

}


/************************************************/
static void JsGcMarkAstCallExpressionNode(void* mp,int ms){
	struct JsAstCallExpressionNode* p = (struct JsAstCallExpressionNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->exp);
	JsGcMark(p->args);
}
struct JsAstCallExpressionNode* JsCreateAstCallExpressionNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstCallExpressionNode),&JsGcMarkAstCallExpressionNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstCallExpressionNode*)node;
}

/************************************************/
static void JsGcMarkAstUnaryNode(void* mp,int ms){
	struct JsAstUnaryNode* p = (struct JsAstUnaryNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->a);
}
struct JsAstUnaryNode* JsCreateAstUnaryNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstUnaryNode),&JsGcMarkAstUnaryNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstUnaryNode*)node;

}


/************************************************/
static void JsGcMarkAstBinaryNode(void* mp,int ms){
	struct JsAstBinaryNode* p = (struct JsAstBinaryNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->a);
	JsGcMark(p->b);
}
struct JsAstBinaryNode* JsCreateAstBinaryNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstBinaryNode),&JsGcMarkAstBinaryNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstBinaryNode*)node;

}

/************************************************/
static void JsGcMarkAstConditionalExpressionNode(void* mp,int ms){
	struct JsAstConditionalExpressionNode* p = (struct JsAstConditionalExpressionNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->a);
	JsGcMark(p->b);
	JsGcMark(p->c);
}
struct JsAstConditionalExpressionNode* JsCreateAstConditionalExpressionNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstConditionalExpressionNode),&JsGcMarkAstConditionalExpressionNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstConditionalExpressionNode*)node;
}

/************************************************/
static void JsGcMarkAstAssignmentExpressionNode(void* mp,int ms){
	struct JsAstAssignmentExpressionNode* p = (struct JsAstAssignmentExpressionNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->lhs);
	JsGcMark(p->expr);
}
struct JsAstAssignmentExpressionNode* JsCreateAstAssignmentExpressionNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstAssignmentExpressionNode),&JsGcMarkAstAssignmentExpressionNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstAssignmentExpressionNode*)node;

}

/************************************************/
static void JsGcMarkAstVariableDeclarationNode(void* mp,int ms){
	struct JsAstVariableDeclarationNode* p = (struct JsAstVariableDeclarationNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->var);
	JsGcMark(p->init);
}
struct JsAstVariableDeclarationNode* JsCreateAstVariableDeclarationNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstVariableDeclarationNode),&JsGcMarkAstVariableDeclarationNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstVariableDeclarationNode*)node;


}
/************************************************/
static void JsGcMarkAstIfStatementNode(void* mp,int ms){
	struct JsAstIfStatementNode* p = (struct JsAstIfStatementNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->cond);
	JsGcMark(p->btrue);
	JsGcMark(p->bfalse);
}
struct JsAstIfStatementNode* JsCreateAstIfStatementNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstIfStatementNode),&JsGcMarkAstIfStatementNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstIfStatementNode*)node;

}

/************************************************/
static void JsGcMarkAstIterationStatementWhileNode(void* mp,int ms){
	struct JsAstIterationStatementWhileNode* p = (struct JsAstIterationStatementWhileNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->cond);
	JsGcMark(p->body);
}

struct JsAstIterationStatementWhileNode* JsCreateAstIterationStatementWhileNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstIterationStatementWhileNode),&JsGcMarkAstIterationStatementWhileNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstIterationStatementWhileNode*)node;

}

/************************************************/
static void JsGcMarkAstIterationStatementForNode(void* mp,int ms){
	struct JsAstIterationStatementForNode* p = (struct JsAstIterationStatementForNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->init);
	JsGcMark(p->cond);
	JsGcMark(p->incr);
	JsGcMark(p->body);
}
struct JsAstIterationStatementForNode* JsCreateAstIterationStatementForNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstIterationStatementForNode),&JsGcMarkAstIterationStatementForNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstIterationStatementForNode*)node;

}

/************************************************/
static void JsGcMarkAstIterationStatementForinNode(void* mp,int ms){
	struct JsAstIterationStatementForinNode* p = (struct JsAstIterationStatementForinNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->lhs);
	JsGcMark(p->list);
	JsGcMark(p->body);
}
struct JsAstIterationStatementForinNode* JsCreateAstIterationStatementForinNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstIterationStatementForinNode),&JsGcMarkAstIterationStatementForinNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstIterationStatementForinNode*)node;
}

/************************************************/

static void JsGcMarkAstContinueStatementNode(void* mp,int ms){
	struct JsAstContinueStatementNode* p = (struct JsAstContinueStatementNode*)mp;
	JsGcMark(p->node.location);
}
struct JsAstContinueStatementNode* JsCreateAstContinueStatementNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstContinueStatementNode),&JsGcMarkAstContinueStatementNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstContinueStatementNode*)node;
}

/************************************************/
static void JsGcMarkAstBreakStatementNode(void* mp,int ms){
	struct JsAstBreakStatementNode* p = (struct JsAstBreakStatementNode*)mp;
	JsGcMark(p->node.location);
}
struct JsAstBreakStatementNode* JsCreateAstBreakStatementNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstBreakStatementNode),&JsGcMarkAstBreakStatementNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstBreakStatementNode*)node;
}

/************************************************/
static void JsGcMarkAstReturnStatementNode(void* mp,int ms){
	struct JsAstReturnStatementNode* p = (struct JsAstReturnStatementNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->expr);
}
struct JsAstReturnStatementNode* JsCreateAstReturnStatementNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstReturnStatementNode),&JsGcMarkAstReturnStatementNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstReturnStatementNode*)node;
}
/************************************************/

static void JsGcMarkAstSwitchStatementNode(void* mp,int ms){
	struct JsAstSwitchStatementNode* p = (struct JsAstSwitchStatementNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->cond);
	JsGcMark(p->cases);
	JsGcMark(p->defcase);
}
struct JsAstSwitchStatementNode* JsCreateAstSwitchStatementNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstSwitchStatementNode),&JsGcMarkAstSwitchStatementNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstSwitchStatementNode*)node;
}
static void JsGcMarkAstCaseList(void* mp,int ms){
	struct JsAstCaseList* p = (struct JsAstCaseList*)mp;
	JsGcMark(p->expr);
	JsGcMark(p->body);
	JsGcMark(p->next);
}
struct JsAstCaseList* JsCreateAstCaseList(){
	struct JsAstCaseList* ele = (struct JsAstCaseList*)JsGcMalloc(sizeof(struct JsAstCaseList),&JsGcMarkAstCaseList,NULL);
	return ele;
}


/************************************************/
static void JsGcMarkAstTryStatementNode(void* mp,int ms){
	struct JsAstTryStatementNode* p = (struct JsAstTryStatementNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->block);
	JsGcMark(p->bcatch);
	JsGcMark(p->bfinally);
	JsGcMark(p->ident);
}
struct JsAstTryStatementNode* JsCreateAstTryStatementNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstTryStatementNode),&JsGcMarkAstTryStatementNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstTryStatementNode*)node;
}

/************************************************/
static void JsGcMarkAstFunctionNode(void* mp,int ms){
	struct JsAstFunctionNode* p = (struct JsAstFunctionNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->argv);
	JsGcMark(p->name);
	JsGcMark(p->body);
}
struct JsAstFunctionNode* JsCreateAstFunctionNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstFunctionNode),&JsGcMarkAstFunctionNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstFunctionNode*)node;
}

/************************************************/
static void JsGcMarkAstFunctionBodyNode(void* mp,int ms){
	struct JsAstFunctionBodyNode* p = (struct JsAstFunctionBodyNode*)mp;
	JsGcMark(p->u.node.location);
	JsGcMark(p->u.a);
}
struct JsAstFunctionBodyNode* JsCreateAstFunctionBodyNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstFunctionBodyNode),&JsGcMarkAstFunctionBodyNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstFunctionBodyNode*)node;

}

/************************************************/
static void JsGcMarkAstSourceElementsNode(void* mp,int ms){
	struct JsAstSourceElementsNode* p = (struct JsAstSourceElementsNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->statements);
}
struct JsAstSourceElementsNode* JsCreateAstSourceElementsNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstSourceElementsNode),&JsGcMarkAstSourceElementsNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstSourceElementsNode*)node;
}
static void JsGcMarkAstSourceElement(void* mp,int ms){
	struct JsAstSourceElement* p = (struct JsAstSourceElement*)mp;
	JsGcMark(p->node);
	JsGcMark(p->next);
}
struct JsAstSourceElement* JsCreateAstSourceElement(){
	struct JsAstSourceElement* ele = (struct JsAstSourceElement*)JsGcMalloc(sizeof(struct JsAstSourceElement),&JsGcMarkAstSourceElement,NULL);
	return ele;
}

/************************************************/
static void JsGcMarkAstSyncBlockStatementNode(void* mp,int ms){
	struct JsAstSyncBlockStatementNode* p = (struct JsAstSyncBlockStatementNode*)mp;
	JsGcMark(p->node.location);
	JsGcMark(p->ident);
	JsGcMark(p->a);
}
struct JsAstSyncBlockStatementNode* JsCreateAstSyncBlockStatementNode(enum JsAstClassEnum type,struct JsLocation* l){
	struct JsAstNode* node =(struct JsAstNode*) JsGcMalloc(sizeof(struct JsAstSyncBlockStatementNode),&JsGcMarkAstSyncBlockStatementNode,NULL);
	node->astClass = type;
	node->location = l;
	return (struct JsAstSyncBlockStatementNode*)node;

}

