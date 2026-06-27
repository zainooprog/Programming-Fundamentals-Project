// ==================libraries includes ==============
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ===================== FILE NAMES =====================
#define PRODUCTS_FILE   "products.txt"
#define CUSTOMERS_FILE  "customers.txt"
#define ORDERS_FILE     "orders.txt"
#define SALES_FILE      "sales_log.txt"
#define EMPLOYEES_FILE  "employees.txt"
#define PENDING_FILE    "pending_orders.txt"
#define MAX             50

// ===================== DATA STORAGE (ARRAYS) =====================
char  productNames[MAX][MAX];
float productPrices[MAX];
int   productQuantities[MAX];
int   totalProducts = 0;

char customerNames[MAX][MAX];
char customerPasswords[MAX][MAX];
int  totalCustomers = 0;
int  loggedInCustomer = -1;

// ===================== SHOP EMPLOYEE DATA =====================
char employeeNames[MAX][MAX];
char employeePasswords[MAX][MAX];
int  totalEmployees = 0;
int  loggedInEmployee = -1;

// ===================== PENDING ORDERS (for approval) =====================
char  pendingCustomer[MAX][MAX];
float pendingTotal[MAX];
int   pendingApproved[MAX];   // 0 = pending, 1 = approved, 2 = rejected
int   totalPendingOrders = 0;

int   cartItems[MAX];
int   cartQty[MAX];
int   cartCount = 0;

// ===================== POINTER UTILITY FUNCTIONS =====================

char* getProductName(int index) {
    if (index < 0 || index >= totalProducts) 
	return NULL;
    return productNames[index];
}

float* getProductPrice(int index) {
    if (index < 0 || index >= totalProducts) 
	return NULL;
    return &productPrices[index];
}

int* getProductQuantity(int index) {
    if (index < 0 || index >= totalProducts) 
	return NULL;
    return &productQuantities[index];
}

void updatePriceByPointer(float *pricePtr, float newPrice) {
    if (pricePtr != NULL) 
	*pricePtr = newPrice;
}

void updateQuantityByPointer(int *qtyPtr, int newQty) {
    if (qtyPtr != NULL) 
	*qtyPtr = newQty;
}

float calculateCartTotal(int *items, int *quantities, int count) {
    float total = 0;
    int i;
    for (i = 0; i < count; i++)
        total += quantities[i] * productPrices[items[i]];
    return total;
}

int searchByPointer(char (*namesArray)[MAX], int size, char *keyword) {
    int i;
    for (i = 0; i < size; i++)
        if (strstr(namesArray[i], keyword) != NULL) 
		return i;
    return -1;
}

// ===================== ARRAY UTILITY FUNCTIONS =====================

void getAllPrices(float *outputArray, int *count) {
    int i;
    *count = totalProducts;
    for (i = 0; i < totalProducts; i++)
        outputArray[i] = productPrices[i];
}

int findMostExpensive(float *pricesArray, int size) {
    int maxIndex = 0, i;
    for (i = 1; i < size; i++)
        if (pricesArray[i] > pricesArray[maxIndex]) 
		maxIndex = i;
    return maxIndex;
}

void sortProductsByPrice() {
    int i, j;
    for (i = 0; i < totalProducts - 1; i++) {
        for (j = 0; j < totalProducts - i - 1; j++) {
            if (productPrices[j] > productPrices[j + 1]) {
                float tempPrice = productPrices[j];
                productPrices[j] = productPrices[j + 1];
                productPrices[j + 1] = tempPrice;

                int tempQty = productQuantities[j];
                productQuantities[j] = productQuantities[j + 1];
                productQuantities[j + 1] = tempQty;

                char tempName[MAX];
                strcpy(tempName, productNames[j]);
                strcpy(productNames[j], productNames[j + 1]);
                strcpy(productNames[j + 1], tempName);
            }
        }
    }
    printf("Products sorted by price (low to high).\n");
}

// ===================== FILE HANDLING FUNCTIONS =====================

void saveProducts() {
    FILE *fp = fopen(PRODUCTS_FILE, "w");
    if (fp == NULL) { printf("Error: Could not save products!\n"); return; }
    fprintf(fp, "%d\n", totalProducts);
    int i;
    for (i = 0; i < totalProducts; i++)
        fprintf(fp, "%s\n%.2f\n%d\n", productNames[i], productPrices[i], productQuantities[i]);
    fclose(fp);
}

void loadProducts() {
    FILE *fp = fopen(PRODUCTS_FILE, "r");
    if (fp == NULL) {
        strcpy(productNames[0], "Chocolate Cake");
        productPrices[0] = 850.00; 
		productQuantities[0] = 10;
		
        strcpy(productNames[1], "Croissant");
        productPrices[1] = 120.00; 
		productQuantities[1] = 25;
		
        strcpy(productNames[2], "Donuts (Pack of 6)");
        productPrices[2] = 350.00; 
		productQuantities[2] = 15;
		
        strcpy(productNames[3], "Cinnamon Roll");
        productPrices[3] = 180.00; 
		productQuantities[3] = 20;
        
        strcpy(productNames[4], "Brownie");
        productPrices[4] = 200.00; 
		productQuantities[4] = 30;
		
        totalProducts = 5;
        saveProducts();
        return;
    }
    fscanf(fp, "%d\n", &totalProducts);
    int i;
    for (i = 0; i < totalProducts; i++) {
        fgets(productNames[i], MAX, fp);
        productNames[i][strcspn(productNames[i], "\n")] = 0;
        fscanf(fp, "%f\n%d\n", &productPrices[i], &productQuantities[i]);
    }
    fclose(fp);
}

void saveCustomers() {
    FILE *fp = fopen(CUSTOMERS_FILE, "w");
    if (fp == NULL) { printf("Error: Could not save customers!\n"); return; }
    fprintf(fp, "%d\n", totalCustomers);
    int i;
    for (i = 0; i < totalCustomers; i++)
        fprintf(fp, "%s\n%s\n", customerNames[i], customerPasswords[i]);
    fclose(fp);
}

void loadCustomers() {
    FILE *fp = fopen(CUSTOMERS_FILE, "r");
    if (fp == NULL) { totalCustomers = 0; return; }
    fscanf(fp, "%d\n", &totalCustomers);
    int i;
    for (i = 0; i < totalCustomers; i++) {
        fgets(customerNames[i], MAX, fp);
        customerNames[i][strcspn(customerNames[i], "\n")] = 0;
        fgets(customerPasswords[i], MAX, fp);
        customerPasswords[i][strcspn(customerPasswords[i], "\n")] = 0;
    }
    fclose(fp);
}

// ===================== EMPLOYEE FILE FUNCTIONS =====================

void saveEmployees() {
    FILE *fp = fopen(EMPLOYEES_FILE, "w");
    if (fp == NULL) { printf("Error: Could not save employees!\n"); return; }
    fprintf(fp, "%d\n", totalEmployees);
    int i;
    for (i = 0; i < totalEmployees; i++)
        fprintf(fp, "%s\n%s\n", employeeNames[i], employeePasswords[i]);
    fclose(fp);
}

void loadEmployees() {
    FILE *fp = fopen(EMPLOYEES_FILE, "r");
    if (fp == NULL) {
        // Default employee account
        strcpy(employeeNames[0], "employee1");
        strcpy(employeePasswords[0], "emp123");
        totalEmployees = 1;
        saveEmployees();
        return;
    }
    fscanf(fp, "%d\n", &totalEmployees);
    int i;
    for (i = 0; i < totalEmployees; i++) {
        fgets(employeeNames[i], MAX, fp);
        employeeNames[i][strcspn(employeeNames[i], "\n")] = 0;
        fgets(employeePasswords[i], MAX, fp);
        employeePasswords[i][strcspn(employeePasswords[i], "\n")] = 0;
    }
    fclose(fp);
}

// ===================== PENDING ORDERS FILE FUNCTIONS =====================

void savePendingOrders() {
    FILE *fp = fopen(PENDING_FILE, "w");
    if (fp == NULL) { printf("Error: Could not save pending orders!\n"); return; }
    fprintf(fp, "%d\n", totalPendingOrders);
    int i;
    for (i = 0; i < totalPendingOrders; i++)
        fprintf(fp, "%s\n%.2f\n%d\n", pendingCustomer[i], pendingTotal[i], pendingApproved[i]);
    fclose(fp);
}

void loadPendingOrders() {
    FILE *fp = fopen(PENDING_FILE, "r");
    if (fp == NULL) { totalPendingOrders = 0; return; }
    fscanf(fp, "%d\n", &totalPendingOrders);
    int i;
    for (i = 0; i < totalPendingOrders; i++) {
        fgets(pendingCustomer[i], MAX, fp);
        pendingCustomer[i][strcspn(pendingCustomer[i], "\n")] = 0;
        fscanf(fp, "%f\n%d\n", &pendingTotal[i], &pendingApproved[i]);
    }
    fclose(fp);
}

void logOrder(float totalAmount) {
    FILE *fp = fopen(SALES_FILE, "a");
    if (fp == NULL) { printf("Warning: Could not write to sales log!\n"); return; }
    fprintf(fp, "==========================================\n");
    fprintf(fp, "Customer : %s\n", customerNames[loggedInCustomer]);
    fprintf(fp, "------------------------------------------\n");
    fprintf(fp, "%-25s %-6s %-10s\n", "Item", "Qty", "Amount");
    fprintf(fp, "------------------------------------------\n");
    int i;
    for (i = 0; i < cartCount; i++) {
        int idx = cartItems[i];
        float subtotal = cartQty[i] * productPrices[idx];
        fprintf(fp, "%-25s %-6d Rs %.2f\n", productNames[idx], cartQty[i], subtotal);
    }
    fprintf(fp, "------------------------------------------\n");
    fprintf(fp, "TOTAL    : Rs %.2f\n", totalAmount);
    fprintf(fp, "STATUS   : Pending Approval\n");
    fprintf(fp, "==========================================\n\n");
    fclose(fp);
    printf("Order saved to sales log.\n");
}

void viewSalesLog() {
    FILE *fp = fopen(SALES_FILE, "r");
    if (fp == NULL) { printf("No sales records found yet.\n"); return; }
    printf("\n===== SALES LOG =====\n");
    char line[200];
    while (fgets(line, sizeof(line), fp) != NULL) printf("%s", line);
    fclose(fp);
}

// ===================== FUNCTION PROTOTYPES =====================
void adminMenu();
void shopEmployeeMenu();
void customerMenu();
void addProduct();
void deleteProduct();
void searchProduct();
void updateProduct();
void viewInventory();
void customerSignUp();
int  customerLogin();
int  employeeLogin();
void browseMenu();
void addToCart();
void viewCart();
void placeOrder();
void displayBill();

// Employee product functions
void empAddProduct();
void empDeleteProduct();
void empSearchProduct();
void empUpdateProduct();
void empViewInventory();

// Employee order functions
void verifyPayment();
void approveOrderRequest();

// ===================== MAIN =====================
int main() {
    printf("Loading data...\n");
    loadProducts();
    loadCustomers();
    loadEmployees();
    loadPendingOrders();

    int choice;
    do {
        printf("\n==========================================\n");
        printf("        SWEET CRAVE CORNER\n");
        printf("    Bakery Shop Management System\n");
        printf("==========================================\n");
        printf("1. Admin\n");
        printf("2. Shop Employee\n");
        printf("3. Customer\n");
        printf("4. Exit\n");
        printf("==========================================\n");
        printf("Enter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: adminMenu();        break;
            case 2: shopEmployeeMenu(); break;
            case 3: customerMenu();     break;
            case 4:
                printf("\nThank you for visiting Sweet Crave Corner!\n");
                printf("Have a sweet day! Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice! Please enter 1, 2, 3, or 4.\n");
        }
    } while (choice != 4);

    return 0;
}

// ===================== ADMIN MENU =====================
void adminMenu() {
    char password[20];
    printf("\n===== ADMIN SECTION =====\n");
    printf("Enter Admin Password: ");
    scanf("%s", password);

    if (strcmp(password, "admin123") != 0) {
        printf("Incorrect password! Access Denied.\n");
        return;
    }
    printf("Admin Login Successful!\n");

    int choice;
    do {
        printf("\n===== ADMIN PANEL =====\n");
        printf("1. Add Product\n");
        printf("2. Delete Product\n");
        printf("3. Search Product\n");
        printf("4. Update Product\n");
        printf("5. View Inventory\n");
        printf("6. View Sales Log\n");
        printf("7. Sort Products by Price\n");
        printf("8. Show Most Expensive Item\n");
        printf("9. Logout\n");
        printf("========================\n");
        printf("Enter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addProduct();    break;
            case 2: deleteProduct(); break;
            case 3: searchProduct(); break;
            case 4: updateProduct(); break;
            case 5: viewInventory(); break;
            case 6: viewSalesLog();  break;
            case 7:
                sortProductsByPrice();
                viewInventory();
                break;
            case 8: {
                float prices[MAX];
                int count;
                getAllPrices(prices, &count);
                int idx = findMostExpensive(prices, count);
                printf("\nMost Expensive Item:\n");
                printf("Name  : %s\n",      getProductName(idx));
                printf("Price : Rs %.2f\n", *getProductPrice(idx));
                break;
            }
            case 9: printf("\nAdmin logged out successfully.\n"); break;
            default: printf("\nInvalid choice!\n");
        }
    } while (choice != 9);
}

// ===================== SHOP EMPLOYEE LOGIN =====================
#define EMPLOYEE_PASSWORD "employ321"

char currentEmployeeName[MAX];

int employeeLogin() {
    printf("\n===== SHOP EMPLOYEE LOGIN =====\n");
    char name[MAX], pass[MAX];
    printf("Enter Employee Name: ");
    scanf(" %[^\n]", name);
    printf("Enter Password: ");
    scanf("%s", pass);

    if (strcmp(pass, EMPLOYEE_PASSWORD) == 0) {
        strcpy(currentEmployeeName, name);
        loggedInEmployee = 0;
        printf("\nLogin Successful! Welcome, %s!\n", currentEmployeeName);
        return 1;
    }

    printf("\nInvalid password! Access Denied.\n");
    return 0;
}

// ===================== SHOP EMPLOYEE MENU =====================
void shopEmployeeMenu() {
    printf("\n===== SHOP EMPLOYEE SECTION =====\n");
    int result = employeeLogin();
    if (result != 1) return;

    int choice;
    do {
        printf("\n===== EMPLOYEE PANEL =====\n");
        printf("1.  Add Product\n");
        printf("2.  Delete Product\n");
        printf("3.  Search Product\n");
        printf("4.  View Inventory\n");
        printf("5.  Update Product\n");
        printf("6.  Verify Payment\n");
        printf("7.  Approve Order Request\n");
        printf("8.  View Sales Log\n");
        printf("9.  Logout\n");
        printf("==========================\n");
        printf("Enter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: empAddProduct();      break;
            case 2: empDeleteProduct();   break;
            case 3: empSearchProduct();   break;
            case 4: empViewInventory();   break;
            case 5: empUpdateProduct();   break;
            case 6: verifyPayment();      break;
            case 7: approveOrderRequest(); break;
            case 8: viewSalesLog();       break;
            case 9:
                loggedInEmployee = -1;
                printf("\nEmployee logged out successfully.\n");
                break;
            default: printf("\nInvalid choice!\n");
        }
    } while (choice != 9);
}

// ===================== EMPLOYEE - ADD PRODUCT =====================
void empAddProduct() {
    if (totalProducts >= MAX) { printf("Storage full!\n"); return; }
    printf("\n===== ADD NEW PRODUCT (Employee) =====\n");
    printf("Enter Product Name: ");
    scanf(" %[^\n]", productNames[totalProducts]);
    printf("Enter Price (Rs): ");
    scanf("%f", &productPrices[totalProducts]);
    printf("Enter Quantity: ");
    scanf("%d", &productQuantities[totalProducts]);
    totalProducts++;
    saveProducts();
    printf("Product Added & Saved Successfully!\n");
}

// ===================== EMPLOYEE - DELETE PRODUCT =====================
void empDeleteProduct() {
    printf("\n===== DELETE PRODUCT (Employee) =====\n");
    empViewInventory();
    if (totalProducts == 0) return;

    int id;
    printf("Enter Product Number to Delete: ");
    scanf("%d", &id);
    id--;

    if (id < 0 || id >= totalProducts) { printf("Invalid product number!\n"); return; }

    int i;
    for (i = id; i < totalProducts - 1; i++) {
        strcpy(productNames[i], productNames[i + 1]);
        productPrices[i]     = productPrices[i + 1];
        productQuantities[i] = productQuantities[i + 1];
    }
    totalProducts--;
    saveProducts();
    printf("Product Deleted & File Updated Successfully!\n");
}

// ===================== EMPLOYEE - SEARCH PRODUCT =====================
void empSearchProduct() {
    printf("\n===== SEARCH PRODUCT (Employee) =====\n");
    char keyword[MAX];
    printf("Enter Product Name to Search: ");
    scanf(" %[^\n]", keyword);

    int idx = searchByPointer(productNames, totalProducts, keyword);
    if (idx != -1) {
        char  *namePtr  = getProductName(idx);
        float *pricePtr = getProductPrice(idx);
        int   *qtyPtr   = getProductQuantity(idx);
        printf("\nProduct Found!\n");
        printf("---------------------------\n");
        printf("Name     : %s\n",      namePtr);
        printf("Price    : Rs %.2f\n", *pricePtr);
        printf("Quantity : %d\n",      *qtyPtr);
        printf("---------------------------\n");
    } else {
        printf("No product found with that name.\n");
    }
}

// ===================== EMPLOYEE - UPDATE PRODUCT =====================
void empUpdateProduct() {
    printf("\n===== UPDATE PRODUCT (Employee) =====\n");
    empViewInventory();
    if (totalProducts == 0) return;

    int id;
    printf("Enter Product Number to Update: ");
    scanf("%d", &id);
    id--;

    if (id < 0 || id >= totalProducts) { printf("Invalid product number!\n"); return; }

    float *pricePtr = getProductPrice(id);
    int   *qtyPtr   = getProductQuantity(id);

    printf("Current Name     : %s\n",      productNames[id]);
    printf("Current Price    : Rs %.2f\n", *pricePtr);
    printf("Current Quantity : %d\n",      *qtyPtr);

    printf("\nEnter New Name: ");
    char temp[MAX];
    scanf(" %[^\n]", temp);
    if (strlen(temp) > 0) strcpy(productNames[id], temp);

    printf("Enter New Price (0 to keep): ");
    float p; scanf("%f", &p);
    if (p > 0) updatePriceByPointer(pricePtr, p);

    printf("Enter New Quantity (0 to keep): ");
    int q; scanf("%d", &q);
    if (q > 0) updateQuantityByPointer(qtyPtr, q);

    saveProducts();
    printf("Product Updated & Saved to File!\n");
}

// ===================== EMPLOYEE - VIEW INVENTORY =====================
void empViewInventory() {
    printf("\n===== PRODUCT INVENTORY (Employee View) =====\n");
    if (totalProducts == 0) { printf("No products available.\n"); return; }
    printf("%-5s %-25s %-12s %-10s\n", "No.", "Product Name", "Price (Rs)", "Quantity");
    printf("----------------------------------------------------\n");
    int i;
    for (i = 0; i < totalProducts; i++) {
        char  *name  = getProductName(i);
        float *price = getProductPrice(i);
        int   *qty   = getProductQuantity(i);
        printf("%-5d %-25s %-12.2f %-10d\n", i + 1, name, *price, *qty);
    }
    printf("----------------------------------------------------\n");
}

// ===================== EMPLOYEE - VERIFY PAYMENT =====================
void verifyPayment() {
    printf("\n===== VERIFY PAYMENT =====\n");

    FILE *fp = fopen(SALES_FILE, "r");
    if (fp == NULL) { printf("No sales records found.\n"); return; }

    printf("Reading sales log for payment verification...\n\n");
    char line[200];
    int orderFound = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
        if (strstr(line, "STATUS") != NULL) orderFound = 1;
    }
    fclose(fp);

    if (!orderFound) {
        printf("No orders pending payment verification.\n");
        return;
    }

    printf("\n------------------------------------------\n");
    printf("Pending Orders for Payment Verification:\n");
    printf("------------------------------------------\n");

    int i, found = 0;
    for (i = 0; i < totalPendingOrders; i++) {
        if (pendingApproved[i] == 0) {
            printf("%d. Customer: %-20s | Total: Rs %.2f | Status: PENDING\n",
                   i + 1, pendingCustomer[i], pendingTotal[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("No pending orders to verify.\n");
        return;
    }

    int orderNo;
    printf("\nEnter Order Number to Verify Payment (0 to cancel): ");
    scanf("%d", &orderNo);

    if (orderNo == 0) return;
    orderNo--;

    if (orderNo < 0 || orderNo >= totalPendingOrders) {
        printf("Invalid order number!\n");
        return;
    }

    if (pendingApproved[orderNo] != 0) {
        printf("This order has already been processed.\n");
        return;
    }

    printf("\nOrder Details:\n");
    printf("Customer : %s\n", pendingCustomer[orderNo]);
    printf("Total    : Rs %.2f\n", pendingTotal[orderNo]);

    int vChoice;
    printf("\n1. Mark Payment as Verified\n");
    printf("2. Mark Payment as Failed\n");
    printf("Enter Choice: ");
    scanf("%d", &vChoice);

    if (vChoice == 1) {
        printf("Payment VERIFIED for customer: %s\n", pendingCustomer[orderNo]);
        printf("Order is now ready for approval.\n");
    } else if (vChoice == 2) {
        pendingApproved[orderNo] = 2;
        savePendingOrders();
        printf("Payment FAILED. Order rejected for customer: %s\n", pendingCustomer[orderNo]);
    } else {
        printf("Invalid choice!\n");
    }
}

// ===================== EMPLOYEE - APPROVE ORDER REQUEST =====================
void approveOrderRequest() {
    printf("\n===== APPROVE ORDER REQUEST =====\n");

    if (totalPendingOrders == 0) {
        printf("No pending order requests.\n");
        return;
    }

    printf("%-5s %-20s %-15s %-12s\n", "No.", "Customer", "Total (Rs)", "Status");
    printf("----------------------------------------------------\n");

    int i, found = 0;
    for (i = 0; i < totalPendingOrders; i++) {
        char *statusStr;
        if      (pendingApproved[i] == 0) statusStr = "PENDING";
        else if (pendingApproved[i] == 1) statusStr = "APPROVED";
        else                              statusStr = "REJECTED";

        printf("%-5d %-20s %-15.2f %-12s\n",
               i + 1, pendingCustomer[i], pendingTotal[i], statusStr);
        if (pendingApproved[i] == 0) found = 1;
    }
    printf("----------------------------------------------------\n");

    if (!found) {
        printf("No pending orders to approve.\n");
        return;
    }

    int orderNo;
    printf("\nEnter Order Number to Approve/Reject (0 to cancel): ");
    scanf("%d", &orderNo);

    if (orderNo == 0) return;
    orderNo--;

    if (orderNo < 0 || orderNo >= totalPendingOrders) {
        printf("Invalid order number!\n");
        return;
    }

    if (pendingApproved[orderNo] != 0) {
        printf("This order has already been processed.\n");
        return;
    }

    printf("\nOrder from: %s | Total: Rs %.2f\n",
           pendingCustomer[orderNo], pendingTotal[orderNo]);

    int aChoice;
    printf("1. Approve Order\n");
    printf("2. Reject Order\n");
    printf("Enter Choice: ");
    scanf("%d", &aChoice);

    if (aChoice == 1) {
        pendingApproved[orderNo] = 1;
        savePendingOrders();
        printf("Order APPROVED for customer: %s\n", pendingCustomer[orderNo]);
        printf("Order will be prepared and delivered.\n");

        // Log the approval to sales file
        FILE *fp = fopen(SALES_FILE, "a");
        if (fp != NULL) {
            fprintf(fp, ">> ORDER APPROVED by Employee: %s\n", currentEmployeeName);
            fprintf(fp, ">> Customer: %s | Total: Rs %.2f\n\n",
                    pendingCustomer[orderNo], pendingTotal[orderNo]);
            fclose(fp);
        }
    } else if (aChoice == 2) {
        pendingApproved[orderNo] = 2;
        savePendingOrders();
        printf("Order REJECTED for customer: %s\n", pendingCustomer[orderNo]);
    } else {
        printf("Invalid choice!\n");
    }
}

// ===================== ADD PRODUCT (Admin) =====================
void addProduct() {
    if (totalProducts >= MAX) { printf("Storage full!\n"); return; }
    printf("\n===== ADD NEW PRODUCT =====\n");
    printf("Enter Product Name: ");
    scanf(" %[^\n]", productNames[totalProducts]);
    printf("Enter Price (Rs): ");
    scanf("%f", &productPrices[totalProducts]);
    printf("Enter Quantity: ");
    scanf("%d", &productQuantities[totalProducts]);
    totalProducts++;
    saveProducts();
    printf("Product Added & Saved Successfully!\n");
}

// ===================== DELETE PRODUCT (Admin) =====================
void deleteProduct() {
    printf("\n===== DELETE PRODUCT =====\n");
    viewInventory();
    if (totalProducts == 0) return;

    int id;
    printf("Enter Product Number to Delete: ");
    scanf("%d", &id);
    id--;

    if (id < 0 || id >= totalProducts) { printf("Invalid product number!\n"); return; }

    int i;
    for (i = id; i < totalProducts - 1; i++) {
        strcpy(productNames[i], productNames[i + 1]);
        productPrices[i]     = productPrices[i + 1];
        productQuantities[i] = productQuantities[i + 1];
    }
    totalProducts--;
    saveProducts();
    printf("Product Deleted & File Updated Successfully!\n");
}

// ===================== SEARCH PRODUCT (Admin) =====================
void searchProduct() {
    printf("\n===== SEARCH PRODUCT =====\n");
    char keyword[MAX];
    printf("Enter Product Name to Search: ");
    scanf(" %[^\n]", keyword);

    int idx = searchByPointer(productNames, totalProducts, keyword);
    if (idx != -1) {
        char  *namePtr  = getProductName(idx);
        float *pricePtr = getProductPrice(idx);
        int   *qtyPtr   = getProductQuantity(idx);
        printf("\nProduct Found!\n");
        printf("---------------------------\n");
        printf("Name     : %s\n",      namePtr);
        printf("Price    : Rs %.2f\n", *pricePtr);
        printf("Quantity : %d\n",      *qtyPtr);
        printf("---------------------------\n");
    } else {
        printf("No product found with that name.\n");
    }
}

// ===================== UPDATE PRODUCT (Admin) =====================
void updateProduct() {
    printf("\n===== UPDATE PRODUCT =====\n");
    viewInventory();
    if (totalProducts == 0) return;

    int id;
    printf("Enter Product Number to Update: ");
    scanf("%d", &id);
    id--;

    if (id < 0 || id >= totalProducts) { printf("Invalid product number!\n"); return; }

    float *pricePtr = getProductPrice(id);
    int   *qtyPtr   = getProductQuantity(id);

    printf("Current Name     : %s\n",      productNames[id]);
    printf("Current Price    : Rs %.2f\n", *pricePtr);
    printf("Current Quantity : %d\n",      *qtyPtr);

    printf("\nEnter New Name: ");
    char temp[MAX];
    scanf(" %[^\n]", temp);
    if (strlen(temp) > 0) strcpy(productNames[id], temp);

    printf("Enter New Price (0 to keep): ");
    float p; scanf("%f", &p);
    if (p > 0) updatePriceByPointer(pricePtr, p);

    printf("Enter New Quantity (0 to keep): ");
    int q; scanf("%d", &q);
    if (q > 0) updateQuantityByPointer(qtyPtr, q);

    saveProducts();
    printf("Product Updated & Saved to File!\n");
}

// ===================== VIEW INVENTORY (Admin) =====================
void viewInventory() {
    printf("\n===== PRODUCT INVENTORY =====\n");
    if (totalProducts == 0) { printf("No products available.\n"); return; }
    printf("%-5s %-25s %-12s %-10s\n", "No.", "Product Name", "Price (Rs)", "Quantity");
    printf("----------------------------------------------------\n");
    int i;
    for (i = 0; i < totalProducts; i++) {
        char  *name  = getProductName(i);
        float *price = getProductPrice(i);
        int   *qty   = getProductQuantity(i);
        printf("%-5d %-25s %-12.2f %-10d\n", i + 1, name, *price, *qty);
    }
    printf("----------------------------------------------------\n");
}

// ===================== CUSTOMER MENU =====================
void customerMenu() {
    int choice;
    printf("\n===== CUSTOMER SECTION =====\n");
    printf("1. Login\n");
    printf("2. Sign Up\n");
    printf("Enter Your Choice: ");
    scanf("%d", &choice);

    if (choice == 2) customerSignUp();

    if (choice == 1 || choice == 2) {
        int result = customerLogin();
        if (result == 1) {
            int opt;
            do {
                printf("\n===== CUSTOMER PANEL =====\n");
                printf("1. Browse Menu\n");
                printf("2. Add to Cart\n");
                printf("3. View Cart\n");
                printf("4. Place Order\n");
                printf("5. Logout\n");
                printf("==========================\n");
                printf("Enter Your Choice: ");
                scanf("%d", &opt);

                switch (opt) {
                    case 1: browseMenu(); break;
                    case 2: addToCart();  break;
                    case 3: viewCart();   break;
                    case 4: placeOrder(); break;
                    case 5:
                        loggedInCustomer = -1;
                        cartCount = 0;
                        printf("\nLogged out successfully. Goodbye!\n");
                        break;
                    default: printf("\nInvalid choice!\n");
                }
            } while (opt != 5);
        }
    }
}

// ===================== CUSTOMER SIGN UP =====================
void customerSignUp() {
    if (totalCustomers >= MAX) { printf("Registration full!\n"); return; }
    printf("\n===== CUSTOMER SIGN UP =====\n");
    printf("Enter Your Name: ");
    scanf(" %[^\n]", customerNames[totalCustomers]);
    printf("Create Password: ");
    scanf("%s", customerPasswords[totalCustomers]);
    totalCustomers++;
    saveCustomers();
    printf("Sign Up Successful! Account saved.\n");
}

// ===================== CUSTOMER LOGIN =====================
int customerLogin() {
    printf("\n===== CUSTOMER LOGIN =====\n");
    char name[MAX], pass[MAX];
    printf("Enter Your Name: ");
    scanf(" %[^\n]", name);
    printf("Enter Password: ");
    scanf("%s", pass);

    int i;
    for (i = 0; i < totalCustomers; i++) {
        if (strcmp(customerNames[i], name) == 0 &&
            strcmp(customerPasswords[i], pass) == 0) {
            loggedInCustomer = i;
            printf("\nLogin Successful! Welcome, %s!\n", customerNames[i]);
            return 1;
        }
    }
    printf("\nInvalid name or password!\n");
    return 0;
}

// ===================== BROWSE MENU =====================
void browseMenu() {
    printf("\n===== BAKERY MENU =====\n");
    viewInventory();
}

// ===================== ADD TO CART =====================
void addToCart() {
    browseMenu();
    if (totalProducts == 0) return;

    int id, qty;
    printf("Enter Product Number to Add: ");
    scanf("%d", &id);
    id--;

    if (id < 0 || id >= totalProducts) { printf("Invalid product number!\n"); return; }

    printf("Enter Quantity: ");
    scanf("%d", &qty);

    int *availQty = getProductQuantity(id);
    if (qty > *availQty) {
        printf("Sorry! Only %d item(s) available.\n", *availQty);
        return;
    }

    int i, found = -1;
    for (i = 0; i < cartCount; i++) {
        if (cartItems[i] == id) { found = i; break; }
    }

    if (found != -1) {
        cartQty[found] += qty;
    } else {
        cartItems[cartCount] = id;
        cartQty[cartCount]   = qty;
        cartCount++;
    }
    printf("%s added to cart!\n", productNames[id]);
}

// ===================== VIEW CART =====================
void viewCart() {
    printf("\n===== YOUR CART =====\n");
    if (cartCount == 0) { printf("Your cart is empty!\n"); return; }

    float total = calculateCartTotal(cartItems, cartQty, cartCount);

    printf("%-5s %-25s %-8s %-10s\n", "No.", "Item", "Qty", "Subtotal");
    printf("----------------------------------------------------\n");
    int i;
    for (i = 0; i < cartCount; i++) {
        int idx = cartItems[i];
        float subtotal = cartQty[i] * productPrices[idx];
        printf("%-5d %-25s %-8d Rs %.2f\n",
               i + 1, productNames[idx], cartQty[i], subtotal);
    }
    printf("----------------------------------------------------\n");
    printf("%-39s Rs %.2f\n", "TOTAL:", total);
    printf("----------------------------------------------------\n");
}

// ===================== PLACE ORDER =====================
void placeOrder() {
    if (cartCount == 0) { printf("\nCart is empty! Please add items first.\n"); return; }

    viewCart();

    int payChoice;
    printf("\n===== PAYMENT METHOD =====\n");
    printf("1. Cash on Delivery (COD)\n");
    printf("2. Online Payment\n");
    printf("Enter Your Choice: ");
    scanf("%d", &payChoice);

    if (payChoice == 1) {
        printf("\nPayment Method: Cash on Delivery\n");
    } else if (payChoice == 2) {
        printf("\nPayment Method: Online Payment\n");
        printf("Payment Verified!\n");
    } else {
        printf("Invalid payment option!\n");
        return;
    }

    displayBill();

    float total = calculateCartTotal(cartItems, cartQty, cartCount);

    // Deduct stock
    int i;
    for (i = 0; i < cartCount; i++) {
        int *qtyPtr = getProductQuantity(cartItems[i]);
        *qtyPtr -= cartQty[i];
    }
    saveProducts();

    // Add to pending orders for employee approval
    if (totalPendingOrders < MAX) {
        strcpy(pendingCustomer[totalPendingOrders], customerNames[loggedInCustomer]);
        pendingTotal[totalPendingOrders]    = total;
        pendingApproved[totalPendingOrders] = 0;
        totalPendingOrders++;
        savePendingOrders();
    }

    logOrder(total);
    cartCount = 0;
    printf("\nOrder Placed Successfully!\n");
    printf("Your order is pending approval by our staff.\n");
    printf("Thank you for shopping at Sweet Crave Corner!\n");
}

// ===================== DISPLAY BILL =====================
void displayBill() {
    printf("\n==========================================\n");
    printf("         SWEET CRAVE CORNER\n");
    printf("             -- BILL --\n");
    printf("==========================================\n");
    printf("Customer : %s\n", customerNames[loggedInCustomer]);
    printf("------------------------------------------\n");
    printf("%-25s %-6s %-10s\n", "Item", "Qty", "Amount");
    printf("------------------------------------------\n");

    float total = 0;
    int i;
    for (i = 0; i < cartCount; i++) {
        int idx = cartItems[i];
        float subtotal = cartQty[i] * productPrices[idx];
        total += subtotal;
        printf("%-25s %-6d Rs %.2f\n", productNames[idx], cartQty[i], subtotal);
    }
    printf("------------------------------------------\n");
    printf("%-32s Rs %.2f\n", "TOTAL AMOUNT:", total);
    printf("==========================================\n");
    printf("       ** Have a Sweet Day! **\n");
    printf("==========================================\n");
}
